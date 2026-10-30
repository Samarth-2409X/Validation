#include "Adc.h"
#include "Adc_Cfg.h"


static uint8 Test_Result;

extern const Adc_ConfigType adcConfig;
extern Adc_ValueGroupType* Adc_GroupResultBufferPtr[];


static Adc_ValueGroupType Test_DataBuffer[10];
static Adc_ValueGroupType *Null_BufferPtr = NULL;

/*******************************************************************************
 * TC 1: Valid Group & Valid Buffer (Normal Operation)
 * Requirement / Requirement Spec: SWS_Adc_00421
 * Description: Checks if API returns E_OK when valid Group and Buffer are passed.
 ******************************************************************************/

void Test_Adc_SetupResultBuffer_Tc_001(void)
{
    Std_ReturnType status;

    /* Initialize ADC module first */
    Adc_Init(&adcConfig);

    /* Call API with valid Group (0) and valid Buffer */
    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    if (status == E_OK)
    {
        Test_Result = 1; 
    }
    else
    {
        Test_Result = 0; 
    }
}

/*******************************************************************************
 * TC 2: Module Uninitialized Check (ADC_E_UNINIT)
 * Requirement / Requirement Spec: SWS_Adc_00125
 * Description: Calling API before Adc_Init should return E_NOT_OK or DET error.
 ******************************************************************************/

void Test_Adc_SetupResultBuffer_Tc_002(void)
{
    Std_ReturnType status;

    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    if (status == E_NOT_OK)
    {
        Test_Result = 1; 
    }
    else
    {
        Test_Result = 0; 
    }
}

/*******************************************************************************
 * TC 3: Invalid Group ID Check (ADC_E_PARAM_GROUP)
 * Requirement / Requirement Spec: SWS_Adc_00126
 * Description: Passing an out-of-bounds Group ID (e.g., 99) should return E_NOT_OK.
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_003(void)
{
    Std_ReturnType status;

    Adc_Init(&adcConfig);

    /* Group 99 is invalid/unconfigured */
    status = Adc_SetupResultBuffer(99, Test_DataBuffer);

    if (status == E_NOT_OK)
    {
        Test_Result = 1;
    }
    else
    {
        Test_Result = 0;
    }
}

/*******************************************************************************
 * TC 4: NULL Pointer Check for DataBufferPtr (ADC_E_PARAM_POINTER)
 * Requirement / Requirement Spec: SWS_Adc_00358
 * Description: Passing NULL pointer as buffer parameter should return E_NOT_OK.
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_004(void)
{
    Std_ReturnType status;

    Adc_Init(&adcConfig);

    /* Pass NULL pointer */
    status = Adc_SetupResultBuffer(0, Null_BufferPtr);

    if (status == E_NOT_OK)
    {
        Test_Result = 1;
    }
    else
    {
        Test_Result = 0;
    }
}

/*******************************************************************************
 * TC 5: Buffer Allocation when Group is Busy (ADC_E_BUSY)
 * Requirement / Requirement Spec: SWS_Adc_00318
 * Description: Cannot update buffer while continuous conversion is ongoing.
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_005(void)
{
    Std_ReturnType status;

    Adc_Init(&adcConfig);
    
    /* Start conversion to put group in ADC_BUSY state */
    Adc_StartGroupConversion(0);

    /* Attempt to reconfigure buffer while busy */
    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    if (status == E_NOT_OK)
    {
        Test_Result = 1; 
    }
    else
    {
        Test_Result = 0; 
    }
    
    /* Cleanup / Stop conversion */
    Adc_StopGroupConversion(0);
}

/*******************************************************************************
 * TC 6: Register Hardware Address Buffer Pointer Verification
 * Requirement / Requirement Spec: SWS_Adc_00422
 * Description: Verifies that internal driver pointer points to target buffer address.
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_006(void)
{
    Adc_Init(&adcConfig);

    Adc_SetupResultBuffer(0, Test_DataBuffer);

    /* Check if internal driver/hardware buffer address matches passed buffer */
    if (Adc_GroupResultBufferPtr[0] == Test_DataBuffer)
    {
        Test_Result = 1; 
    }
    else
    {
        Test_Result = 0; 
    }
}