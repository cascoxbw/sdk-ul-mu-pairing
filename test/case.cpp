#include <gtest/gtest.h>
#include "../lib/UlMuPair.hpp"

class Pair : public ::testing::Test
{
  protected:
    sInput inT = {};
    sOutput outT = {};

    virtual void SetUp()
    {
        inT.ueNum = 16;

        inT.ue[0].id = 1;
        inT.ue[0].layer = 1;
        inT.ue[0].sinrSu = 9;
        inT.ue[0].prb = 16;
        float v0[VectorSize] = 
        {0.2500,0,0.2079,0.1389,0.0957,0.2310,-0.0488,0.2452,
         -0.1768,0.1768,-0.2452,0.0488,-0.2310,-0.0957,-0.1389,-0.2079,
         0.2500,0,0.2079,0.1389,0.0957,0.2310,-0.0488,0.2452,
         -0.1768,0.1768,-0.2452,0.0488,-0.2310,-0.0957,-0.1389,-0.2079};
        memcpy(inT.ue[0].vector, v0, VectorSize*sizeof(float));

        inT.ue[1].id = 2;
        inT.ue[1].layer = 1;
        inT.ue[1].sinrSu = 25;
        inT.ue[1].prb = 27;
        float v1[VectorSize] = 
        {0.2500,0,-0.0957,0.2310,-0.1768,-0.1768,0.2310,-0.0957,
         0,0.2500,-0.2310,-0.0957,0.1768,-0.1768,0.0957,0.2310,
         0.1768,0.1768,-0.2310,0.0957,0,-0.2500,0.2310,0.0957,
         -0.1768,0.1768,-0.0957,-0.2310,0.2500,-0.0000,-0.0957,0.2310};
        memcpy(inT.ue[1].vector, v1, VectorSize*sizeof(float));

        inT.ue[2].id = 3;
        inT.ue[2].layer = 1;
        inT.ue[2].sinrSu = 29;
        inT.ue[2].prb = 16;
        float v2[VectorSize] = 
        {0.2500,0,-0.2310,0.0957,0.1768,-0.1768,-0.0957,0.2310,
         0,-0.2500,0.0957,0.2310,-0.1768,-0.1768,0.2310,0.0957,
         0.2500,0,-0.2310,0.0957,0.1768,-0.1768,-0.0957,0.2310,
         0,-0.2500,0.0957,0.2310,-0.1768,-0.1768,0.2310,0.0957};
        memcpy(inT.ue[2].vector, v2, VectorSize*sizeof(float));

        inT.ue[3].id = 4;
        inT.ue[3].layer = 1;
        inT.ue[3].sinrSu = 12;
        inT.ue[3].prb = 29;
        float v3[VectorSize] = 
        {0.2500,0,0.0488,-0.2452,-0.2310,-0.0957,-0.1389,0.2079,
         0.1768,0.1768,0.2079,-0.1389,-0.0957,-0.2310,-0.2452,0.0488,
         -0.1768,0.1768,0.1389,0.2079,0.2310,-0.0957,-0.0488,-0.2452,
         -0.2500,0,-0.0488,0.2452,0.2310,0.0957,0.1389,-0.2079};
        memcpy(inT.ue[3].vector, v3, VectorSize*sizeof(float));

        inT.ue[4].id = 5;
        inT.ue[4].layer = 1;
        inT.ue[4].sinrSu = 6;
        inT.ue[4].prb = 24;
        float v4[VectorSize] = 
        {0.2500,0,-0.2310,0.0957,0.1768,-0.1768,-0.0957,0.2310,
         0, -0.2500,0.0957,0.2310,-0.1768,-0.1768,0.2310,0.0957,
         0,0.2500,-0.0957,-0.2310,0.1768,0.1768,-0.2310,-0.0957,
         0.2500,0,-0.2310,0.0957,0.1768,-0.1768,-0.0957,0.2310};
        memcpy(inT.ue[4].vector, v4, VectorSize*sizeof(float));

        inT.ue[5].id = 6;
        inT.ue[5].layer = 1;
        inT.ue[5].sinrSu = 20;
        inT.ue[5].prb = 28;
        float v5[VectorSize] = 
        {0.2500,0,0,-0.2500,-0.2500,0,0,0.2500,
         0.2500,0,0,-0.2500,-0.2500,0,0,0.2500,
         0.1768,0.1768,0.1768,-0.1768,-0.1768,-0.1768,-0.1768,0.1768,
         0.1768,0.1768,0.1768,-0.1768,-0.1768,-0.1768,-0.1768,0.1768};
        memcpy(inT.ue[5].vector, v5, VectorSize*sizeof(float));

        inT.ue[6].id = 7;
        inT.ue[6].layer = 1;
        inT.ue[6].sinrSu = 9;
        inT.ue[6].prb = 27;
        float v6[VectorSize] = 
        {0.2500,0,-0.1768,-0.1768,0,0.2500,0.1768,-0.1768,
         -0.2500,0,0.1768,0.1768,0,-0.2500,-0.1768,0.1768,
         0.1768,-0.1768,-0.2500,0,0.1768,0.1768,0,-0.2500,
         -0.1768,0.1768,0.2500,0,-0.1768,-0.1768,0,0.2500};
        memcpy(inT.ue[6].vector, v6, VectorSize*sizeof(float));

        inT.ue[7].id = 8;
        inT.ue[7].layer = 1;
        inT.ue[7].sinrSu = 5;
        inT.ue[7].prb = 18;
        float v7[VectorSize] = 
        {0.2500,0,-0.2452,0.0488,0.2310,-0.0957,-0.2079,0.1389,
         0.1768,-0.1768,-0.1389,0.2079,0.0957,-0.2310,-0.0488,0.2452,
         0.1768,-0.1768,-0.1389,0.2079,0.0957,-0.2310,-0.0488,0.2452,
         0,-0.2500,0.0488,0.2452,-0.0957,-0.2310,0.1389,0.2079};
        memcpy(inT.ue[7].vector, v7, VectorSize*sizeof(float));

        inT.ue[8].id = 9;
        inT.ue[8].layer = 1;
        inT.ue[8].sinrSu = 29;
        inT.ue[8].prb = 21;
        float v8[VectorSize] = 
        {0.2500,0,-0.2310,-0.0957,0.1768,0.1768,-0.0957,-0.2310,
         0,0.2500,0.0957,-0.2310,-0.1768,0.1768,0.2310,-0.0957,
         -0.1768,-0.1768,0.0957,0.2310,0,-0.2500,-0.0957,0.2310,
         0.1768,-0.1768,-0.2310,0.0957,0.2500,0,-0.2310,-0.0957};
        memcpy(inT.ue[8].vector, v8, VectorSize*sizeof(float));

        inT.ue[9].id = 10;
        inT.ue[9].layer = 1;
        inT.ue[9].sinrSu = 7;
        inT.ue[9].prb = 28;
        float v9[VectorSize] = 
        {0.2500,0,0,0.2500,-0.2500,0,0,-0.2500,
         0.2500,0,0,0.2500,-0.2500,0,0,-0.2500,
         0.1768,0.1768,-0.1768,0.1768,-0.1768,-0.1768,0.1768,-0.1768,
         0.1768,0.1768,-0.1768,0.1768,-0.1768,-0.1768,0.1768,-0.1768};
        memcpy(inT.ue[9].vector, v9, VectorSize*sizeof(float));

        inT.ue[10].id = 11;
        inT.ue[10].layer = 1;
        inT.ue[10].sinrSu = 16;
        inT.ue[10].prb = 27;
        float v10[VectorSize] = 
        {0.2500,0,0.1768,-0.1768,0,-0.2500,-0.1768,-0.1768,
         -0.2500,0,-0.1768,0.1768,0,0.2500,0.1768,0.1768,
         -0.2500,0,-0.1768,0.1768,0,0.2500,0.1768,0.1768,
         0.2500,0,0.1768,-0.1768,0,-0.2500,-0.1768,-0.1768};
        memcpy(inT.ue[10].vector, v10, VectorSize*sizeof(float));

        inT.ue[11].id = 12;
        inT.ue[11].layer = 1;
        inT.ue[11].sinrSu = 29;
        inT.ue[11].prb = 25;
        float v11[VectorSize] = 
        {0.2500,0,0.1768,0.1768,0,0.2500,-0.1768,0.1768,
         -0.2500,0,-0.1768,-0.1768,0,-0.2500,0.1768,-0.1768,
         -0.1768,0.1768,-0.2500,0,-0.1768,-0.1768,0,-0.2500,
         0.1768,-0.1768,0.2500,0,0.1768,0.1768,0,0.2500};
        memcpy(inT.ue[11].vector, v11, VectorSize*sizeof(float));

        inT.ue[12].id = 13;
        inT.ue[12].layer = 1;
        inT.ue[12].sinrSu = 5;
        inT.ue[12].prb = 20;
        float v12[VectorSize] = 
        {0.2500,0,0.0957,-0.2310,-0.1768,-0.1768,-0.2310,0.0957,
         0,0.2500,0.2310,0.0957,0.1768,-0.1768,-0.0957,-0.2310,
         0.1768,-0.1768,-0.0957,-0.2310,-0.2500,-0.0957,0.2310,
         0.1768,0.1768,0.2310,-0.0957,0,-0.2500,-0.2310,-0.0957};
        memcpy(inT.ue[12].vector, v12, VectorSize*sizeof(float));

        inT.ue[13].id = 14;
        inT.ue[13].layer = 1;
        inT.ue[13].sinrSu = 8;
        inT.ue[13].prb = 29;
        float v13[VectorSize] = 
        {0.2500,0,0.0488,0.2452,-0.2310,0.0957,-0.1389,-0.2079,
         0.1768,-0.1768,0.2079,0.1389,-0.0957,0.2310,-0.2452,-0.0488,
         0,-0.2500,0.2452,-0.0488,0.0957,0.2310,-0.2079,0.1389,
         -0.1768,-0.1768,0.1389,-0.2079,0.2310,0.0957,-0.0488,0.2452};
        memcpy(inT.ue[13].vector, v13, VectorSize*sizeof(float));

        inT.ue[14].id = 15;
        inT.ue[14].layer = 1;
        inT.ue[14].sinrSu = 28;
        inT.ue[14].prb = 21;
        float v14[VectorSize] = 
        {0.2500,0,0.0957,0.2310,-0.1768,0.1768,-0.2310,-0.0957,
         0,-0.2500,0.2310,-0.0957,0.1768,0.1768,-0.0957,0.2310,
         0,0.2500,-0.2310,0.0957,-0.1768,-0.1768,0.0957,-0.2310,
         0.2500,0,0.0957,0.2310,-0.1768,0.1768,-0.2310,-0.0957};
        memcpy(inT.ue[14].vector, v14, VectorSize*sizeof(float));

        inT.ue[15].id = 16;
        inT.ue[15].layer = 1;
        inT.ue[15].sinrSu = 8;
        inT.ue[15].prb = 26;
        float v15[VectorSize] = 
        {0.2500,0,-0.2452,0.0488,0.2310,-0.0957,-0.2079,0.1389,
         0.1768,-0.1768,-0.1389,0.2079,0.0957,-0.2310,-0.0488,0.2452,
         0.1768,0.1768,-0.2079,-0.1389,0.2310,0.0957,-0.2452,-0.0488,
         0.2500,0,-0.2452,0.0488,0.2310,-0.0957,-0.2079,0.1389};
        memcpy(inT.ue[15].vector, v15, VectorSize*sizeof(float));

        printf("input setup, input ue num=%d\n", inT.ueNum);

        outT.setNum = 10;

        outT.set[0].ueNum = 2;
        outT.set[0].ueId[0] = 1;
        outT.set[0].ueId[1] = 3;
        outT.set[0].sinrMu[0] = 8.6666;
        outT.set[0].sinrMu[1] = 19.4679;
        outT.set[0].prb = 16;
        outT.set[0].corrFactor = 0.01;
        outT.set[0].layerSum = 2;

        outT.set[1].ueNum = 2;
        outT.set[1].ueId[0] = 2;
        outT.set[1].ueId[1] = 4;
        outT.set[1].sinrMu[0] = 21.4836;
        outT.set[1].sinrMu[1] = 11.7367;
        outT.set[1].prb = 27;
        outT.set[1].corrFactor = 0.0039;
        outT.set[1].layerSum = 2;

        outT.set[2].ueNum = 2;
        outT.set[2].ueId[0] = 5;
        outT.set[2].ueId[1] = 12;
        outT.set[2].sinrMu[0] = 5.6787;
        outT.set[2].sinrMu[1] = 16.872;
        outT.set[2].prb = 24;
        outT.set[2].corrFactor = 0.0193;
        outT.set[2].layerSum = 2;

        outT.set[3].ueNum = 2;
        outT.set[3].ueId[0] = 6;
        outT.set[3].ueId[1] = 7;
        outT.set[3].sinrMu[0] = 20;
        outT.set[3].sinrMu[1] = 9;
        outT.set[3].prb = 28;
        outT.set[3].corrFactor = 0.0001;
        outT.set[3].layerSum = 2;

        outT.set[4].ueNum = 1;
        outT.set[4].ueId[0] = 8;
        outT.set[4].ueId[1] = 0;
        outT.set[4].sinrMu[0] = 0;
        outT.set[4].sinrMu[1] = 0;
        outT.set[4].prb = 18;
        outT.set[4].corrFactor = 0;
        outT.set[4].layerSum = 1;

        outT.set[5].ueNum = 2;
        outT.set[5].ueId[0] = 9;
        outT.set[5].ueId[1] = 13;
        outT.set[5].sinrMu[0] = 29;
        outT.set[5].sinrMu[1] = 5;
        outT.set[5].prb = 21;
        outT.set[5].corrFactor = 0.0001;
        outT.set[5].layerSum = 2;

        outT.set[6].ueNum = 2;
        outT.set[6].ueId[0] = 10;
        outT.set[6].ueId[1] = 11;
        outT.set[6].sinrMu[0] = 7;
        outT.set[6].sinrMu[1] = 16;
        outT.set[6].prb = 28;
        outT.set[6].corrFactor = 0.0001;
        outT.set[6].layerSum = 2;

        outT.set[7].ueNum = 1;
        outT.set[7].ueId[0] = 14;
        outT.set[7].ueId[1] = 0;
        outT.set[7].sinrMu[0] = 0;
        outT.set[7].sinrMu[1] = 0;
        outT.set[7].prb = 29;
        outT.set[7].corrFactor = 0;
        outT.set[7].layerSum = 1;

        outT.set[8].ueNum = 1;
        outT.set[8].ueId[0] = 15;
        outT.set[8].ueId[1] = 0;
        outT.set[8].sinrMu[0] = 0;
        outT.set[8].sinrMu[1] = 0;
        outT.set[8].prb = 21;
        outT.set[8].corrFactor = 0;
        outT.set[8].layerSum = 1;

        outT.set[9].ueNum = 1;
        outT.set[9].ueId[0] = 16;
        outT.set[9].ueId[1] = 0;
        outT.set[9].sinrMu[0] = 0;
        outT.set[9].sinrMu[1] = 0;
        outT.set[9].prb = 26;
        outT.set[9].corrFactor = 0;
        outT.set[9].layerSum = 1;

        printf("output setup, output set num=%d\n", outT.setNum);
    }
};

TEST_F(Pair, In3UeOut2Set) 
{
    inT.ueNum = 3;
    sOutput out = {};

    bool result = UlMuPair(&inT,&out);

    EXPECT_EQ(result, true);
    EXPECT_EQ(out.setNum, 2);
    EXPECT_EQ(out.set[0].ueNum, 2);
    EXPECT_EQ(out.set[0].prb, 16);
    EXPECT_EQ(out.set[0].ueId[0], 1);
    EXPECT_EQ(out.set[0].ueId[1], 3);
}

TEST_F(Pair, In16UeOut10Set) 
{
    sOutput out = {};

    bool result = UlMuPair(&inT,&out);

    EXPECT_EQ(result, true);
    EXPECT_EQ(out.setNum, outT.setNum);

    for(int setIndex = 0; setIndex < out.setNum; setIndex++)
    {
        EXPECT_EQ(out.set[setIndex].ueNum, outT.set[setIndex].ueNum);
        EXPECT_EQ(out.set[setIndex].ueId[0], outT.set[setIndex].ueId[0]);
        EXPECT_EQ(out.set[setIndex].ueId[1], outT.set[setIndex].ueId[1]);
        EXPECT_EQ(out.set[setIndex].sinrMu[0], outT.set[setIndex].sinrMu[0]);
        EXPECT_EQ(out.set[setIndex].sinrMu[1], outT.set[setIndex].sinrMu[1]);
        EXPECT_EQ(out.set[setIndex].prb, outT.set[setIndex].prb);
        EXPECT_EQ(out.set[setIndex].corrFactor, outT.set[setIndex].corrFactor);
        EXPECT_EQ(out.set[setIndex].layerSum, outT.set[setIndex].layerSum);
    }
}