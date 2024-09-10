
#define SinrThold   3
#define CorrThold   0.3
#define MaxUeCs1    16
#define MaxPairSet  MaxUeCs1
#define VectorSize  32
#define MinPrbRate  0.9
#define MaxPrbRate  1.1
#define MaxPrb      273
#define MaxPair     2
#define UeLayer     1
#define PairSetLayer MaxPair * UeLayer

#define Min(X, Y) (((X) < (Y)) ? (X) : (Y));

struct sUe
{
    int   id;
    int   layer;
    float sinrSu;
    int   prb;
    float vector[VectorSize];
};

struct sInput
{
    int ueNum;
    sUe ue[MaxUeCs1]; //already sorted according to priority, 1st UE highest priority and last UE least priority
};

struct sPairSet
{
    int   ueNum; //0: set unused, 1: set for su, > 1: set for mu
    int   ueId[MaxPair];
    int   prb;
    float corrFactor[MaxPair];
    float sinrMu[MaxPair];
    int   layerSum;
};

struct sOutput
{
    int      setNum;
    sPairSet set[MaxPairSet];
};

bool UlMuPair(sInput* pIn, sOutput* pOut);