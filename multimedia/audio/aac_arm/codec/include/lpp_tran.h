/*
  Low Power Profile Transposer
*/

#ifndef _LPP_TRANS_H
#define _LPP_TRANS_H

#include "sbrdecsettings.h"
#include "audiolibs_common.h"

#define MAX_NUM_PATCHES 6
#define GUARDBANDS      0
#define SHIFT_START_SB  1

typedef enum
{
  INVF_OFF = 0,
  INVF_LOW_LEVEL,
  INVF_MID_LEVEL,
  INVF_HIGH_LEVEL,
  INVF_SWITCHED
}
INVF_MODE;


typedef struct {
  unsigned char  sourceStartBand;
  unsigned char  sourceStopBand;
  unsigned char  guardStartBand;

  unsigned char  targetStartBand;
  unsigned char  targetBandOffs;
  unsigned char  numBandsInPatch;
} PATCH_PARAM;


typedef struct {

  Float  off;
  Float  transitionLevel;
  Float  lowLevel;
  Float  midLevel;
  Float  highLevel;
} WHITENING_FACTORS;



typedef struct {
  unsigned char  nCols;
  unsigned char  noOfPatches;
  unsigned char  lbStartPatching;
  unsigned char  lbStopPatching;
  unsigned char  bwBorders[MAX_NUM_NOISE_VALUES];	/* 10  */
  PATCH_PARAM    patchParam[MAX_NUM_PATCHES];		/* 6x6 */
  WHITENING_FACTORS whFactors;						/* 5   */

} TRANSPOSER_SETTINGS;


typedef struct
{
  TRANSPOSER_SETTINGS *pSettings;
  Float  bwVectorOld[MAX_NUM_PATCHES];		/* 6 */
  Float  *lpcFilterStatesReal[LPC_ORDER];
#ifndef LP_SBR_ONLY
  Float  *lpcFilterStatesImag[LPC_ORDER];
#endif
#ifdef MMDSP
  int lpcFilterStatesScale;
#endif
}
SBR_LPP_TRANS;

typedef SBR_LPP_TRANS *HANDLE_SBR_LPP_TRANS;


void lppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,
                    Float **qmfBufferReal,
#ifndef LP_SBR_ONLY
                    Float **qmfBufferImag,
#endif
                    Float *degreeAlias,
                    int timeStep,
                    int firstSlotOffset,
                    int lastSlotOffset,
                    unsigned char nInvfBands,
                    INVF_MODE *sbr_invf_mode,
                    INVF_MODE *sbr_invf_mode_prev,
                    int bUseLP
#ifdef MMDSP
                    ,int *scalePtr
                    ,int ov_lb_scale
#endif
                    );


int
createLppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,
                     unsigned char highBandStartSb,
                     unsigned char *v_k_master,
                     unsigned char numMaster,
                     unsigned char usb,
                     unsigned char nCols,
                     unsigned char* noiseBandTable,
                     unsigned char noNoiseBands,
                     unsigned short fs,
                     unsigned char chan);


int
resetLppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,
                    unsigned char xposctrl,
                    unsigned char highBandStartSb,
                    unsigned char *v_k_master,
                    unsigned char numMaster,
                    unsigned char* noiseBandTable,
                    unsigned char noNoiseBands,
                    unsigned char usb,
                    unsigned short fs);



#endif /* _LPP_TRANS_H */

