#include "Adc.h"
#include "Adc_Cfg.h"

/* Result of the last test case: 1 = passed, 0 = failed */
static uint8 Test_Result;

/* Test case for ADC module initialization */
/* Checks that Adc_Init writes the configured sample time (AdcChannelSampTime = 1) */
void Test_Adc_Tc_001(void)
{
    Adc_Init(&adcConfig);
    if (EVADC.SAMPLETIME.U == 1)
    {
        // Test passed
    }
    else
    {
        // Test failed
    }
}

/*SWS_Adc_00307*/
/* Adc_Init shall set all groups to ADC_IDLE state */
void Test_Adc_Tc_002(void)
{
    Adc_Init(&adcConfig);
    if (Adc_GetGroupStatus(ADC_GROUP_ID0) == ADC_IDLE)   /* ADC_IDLE = 0x0U */
    {
        // Test passed
    }
    else
    {
        // Test failed
    }
}

/*Software triggered One-Shot conversion without notification*/
/* Setup buffer, start group, wait for completion, read result */
void Test_Adc_Tc_003(void)
{
    Adc_ValueGroupType resultBuffer[1];
    uint32 timeout = 100000U;

    Adc_Init(&adcConfig);
    Adc_SetupResultBuffer(ADC_GROUP_ID0, resultBuffer);
    Adc_StartGroupConversion(ADC_GROUP_ID0);

    /* Poll until conversion completes (no notification used) */
    while ((Adc_GetGroupStatus(ADC_GROUP_ID0) == ADC_BUSY) && (timeout > 0U))
    {
        timeout--;
    }

    if ((timeout > 0U) && (Adc_ReadGroup(ADC_GROUP_ID0, resultBuffer) == E_OK)
        && (resultBuffer[0] <= 4095U))    /* 12-bit ADC range */
    {
        // Test passed
    }
    else
    {
        // Test failed
    }
}

/*SWS_Adc_00107*/
/* Calling Adc_Init when already initialized shall raise ADC_E_ALREADY_INITIALIZED (DET enabled) */
/* Needs a Det_ReportError() stub that stores the reported error id in Det_LastError */
void Test_Adc_Tc_004(void)
{
    Det_LastError = 0U;

    Adc_Init(&adcConfig);
    Adc_Init(&adcConfig);                 /* second call */

    if (Det_LastError == ADC_E_ALREADY_INITIALIZED)
    {
        // Test passed
    }
    else
    {
        // Test failed
    }
}

/*Adc_StopGroupConversion : group returns to ADC_IDLE*/
/* TODO: add the SWS_Adc_xxxxx ID from the PDF */
void Test_Adc_Tc_005(void)
{
    Adc_ValueGroupType resultBuffer[1];

    Adc_Init(&adcConfig);
    Adc_SetupResultBuffer(ADC_GROUP_ID0, resultBuffer);
    Adc_StartGroupConversion(ADC_GROUP_ID0);
    Adc_StopGroupConversion(ADC_GROUP_ID0);

    if (Adc_GetGroupStatus(ADC_GROUP_ID0) == ADC_IDLE)
    {
        // Test passed
    }
    else
    {
        // Test failed
    }
}


