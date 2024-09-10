#include <stdio.h>
#include <math.h>
#include "UlMuPair.hpp"

/**
 * Calculation of correlation factor
 */
static float UlMuPairCalcCorr(float* vectorA, float* vectorB)
{
    float ab2 = 0;
    float a2 = 0;
    float b2 = 0;

    for (int i = 0; i < VectorSize; i++)
    {
        ab2 += powf(vectorA[i] * vectorB[i], 2);
        a2 += powf(vectorA[i], 2);
        b2 += powf(vectorB[i], 2);
    }

    return ab2 / a2 / b2;
}

/**
 * Calculation of sinr mu
 */
static void UlMuPairCalcSinr(float* vectorA, float* vectorB, float sinrSuA, float sinrSuB, float* sinrMuA, float* sinrMuB)
{
    float ab2 = 0;
    float a2 = 0;
    float b2 = 0;

    for (int i = 0; i < VectorSize; i++)
    {
        ab2 += powf(vectorA[i] * vectorB[i], 2);
        a2 += powf(vectorA[i], 2);
        b2 += powf(vectorB[i], 2);
    }

    float sinrA = powf(10, sinrSuA / 10);
    float sinrB = powf(10, sinrSuB / 10);
    float alpha = 1;

    sinrA = 1 / (1 / sinrA + alpha * ab2 / a2);
    sinrB = 1 / (1 / sinrB + alpha * ab2 / b2);

    *sinrMuA = 10 * log10f(sinrA);
    *sinrMuB = 10 * log10f(sinrB);
}

/**
 * Check sinr mu
 */
static bool UlMuPairCheckSinr(float sinrSuA, float sinrSuB, float sinrMuA, float sinrMuB)
{
    float linearMuA = powf(10, sinrMuA / 10);
    float linearMuB = powf(10, sinrMuB / 10);
    float linearSuA = powf(10, sinrSuA / 10);
    float linearSuB = powf(10, sinrSuB / 10);
    float seSu = (10 * log2f(1 + linearSuA) + 10 * log2f(1 + linearSuB)) / 2;
    float seMu = 10 * log2f(1 + linearMuA) + 10 * log2f(1 + linearMuB);
    return seMu > seSu;
}

/**
 * Pair main program
 */
bool UlMuPair(sInput* pIn, sOutput* pOut)
{
    sUe* pUe = pIn->ue;
    int totalUe = pIn->ueNum;
    sPairSet* pSet = pOut->set;
    pOut->setNum = 0;

    printf("ul mu pair start, input ue num=%d\n", totalUe);

    // parm check
    if (totalUe <= 0 || totalUe > MaxUeCs1)
    {
        printf("input ue num error\n");
        return false;
    }

    // init
    int availPrb = MaxPrb;
    int availUe = 0;
    bool availUeMap[MaxUeCs1];

    for (int ueIndex = 0; ueIndex < totalUe; ueIndex++)
    {
        if (pUe[ueIndex].layer > 0 && pUe[ueIndex].prb > 0 && pUe[ueIndex].sinrSu > SinrThold)
        {
            availUeMap[ueIndex] = true;
            availUe++;
        }
    }

    // pair
    for (int setIndex = 0; setIndex < MaxPairSet; setIndex++)
    {
        if (availUe <= 0 || availPrb <= 0)
        {
            // printf("no resources available, stop pairing\n");
            break;
        }

        // find root ue
        int rootIndex = totalUe;
        for (int ueIndex = 0; ueIndex < totalUe; ueIndex++)
        {
            if (availUeMap[ueIndex] && availPrb >= pUe[ueIndex].prb)
            {
                rootIndex = ueIndex;
                break;
            }
        }

        if (rootIndex == totalUe)
        {
            printf("can't find root ue, stop pairing\n");
            break;
        }

        // fill pairset info
        sPairSet* pSetTemp = pSet + setIndex;
        pSetTemp->ueId[0] = pUe[rootIndex].id;
        pSetTemp->prb = pUe[rootIndex].prb;
        pSetTemp->layerSum = pUe[rootIndex].layer;
        pSetTemp->ueNum = 1;
        pOut->setNum++;

        // maintain temp info
        availUeMap[rootIndex] = false;
        availPrb -= pSetTemp->prb;
        availUe--;

        // finish pair, su set
        if (pSetTemp->layerSum > UeLayer)
        {
            continue;
        }

        // find pair ue
        for (int ueIndex = 0; ueIndex < totalUe; ueIndex++)
        {
            if (!availUeMap[ueIndex])
            {
                continue;
            }

            // check prb
            float prbRate = pUe[ueIndex].prb * 1.0 / pSetTemp->prb;
            if (prbRate < MinPrbRate || prbRate > MaxPrbRate)
            {
                continue;
            }

            // check correlation
            float corr = UlMuPairCalcCorr(pUe[rootIndex].vector, pUe[ueIndex].vector);
            if (corr >= CorrThold)
            {
                continue;
            }

            // check sinr mu
            float sinrMuRoot = 0;
            float sinrMuPair = 0;
            UlMuPairCalcSinr(pUe[rootIndex].vector, pUe[ueIndex].vector, pUe[rootIndex].sinrSu, pUe[ueIndex].sinrSu, 
                             &sinrMuRoot, &sinrMuPair);
            if (!UlMuPairCheckSinr(pUe[rootIndex].sinrSu, pUe[ueIndex].sinrSu, sinrMuRoot, sinrMuPair))
            {
                continue;
            }

            // fill pairset info
            pSetTemp->ueId[pSetTemp->ueNum] = pUe[ueIndex].id;
            pSetTemp->layerSum += pUe[ueIndex].layer;
            pSetTemp->corrFactor[pSetTemp->ueNum] = corr;
            pSetTemp->sinrMu[0] = sinrMuRoot;
            pSetTemp->sinrMu[pSetTemp->ueNum] = sinrMuPair;
            pSetTemp->ueNum++;

            // maintain temp info
            availUeMap[ueIndex] = false;
            availUe--;

            // finish pair, mu set
            if (pSetTemp->ueNum >= MaxPair || pSetTemp->layerSum >= PairSetLayer)
            {
                break;
            }
        }
    }

    printf("ul mu pair end, output set num=%d\n", pOut->setNum);

    return true;
}