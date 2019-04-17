/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== adcBufContinuousSampling.c ========
 */

#include <stdint.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"

#define TASKSTACKSIZE    (768)
#define ADCBUFFERSIZE    (1)
#define UART_WRITE_BUFFER_SIZE (10)
#define UART_READ_BUFFER_SIZE (3)

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

uint16_t sampleBufferCh1One[ADCBUFFERSIZE];
uint16_t sampleBufferCh1Two[ADCBUFFERSIZE];
uint16_t sampleBufferCh2One[ADCBUFFERSIZE];
uint16_t sampleBufferCh2Two[ADCBUFFERSIZE];
uint32_t microVoltBuffer[ADCBUFFERSIZE];
uint32_t buffersCompletedCounter = 0;
uint_fast16_t uartOutputBufferSize = 0;

char uartWriteBuffer[UART_WRITE_BUFFER_SIZE];
char uartReadBuffer[UART_READ_BUFFER_SIZE];
char endLineSequence[UART_READ_BUFFER_SIZE] = "end";

uint32_t lastValue = NULL;
uint32_t time = 0;

ADCBuf_Conversion continuousConversionChannel[2];
int channelOne = 1;
int channelTwo = 2;

/* Driver handle shared between the task and the callback function */
UART_Handle uart;


/*
 * This function is called whenever a buffer is full.
 * The content of the buffer is then converted into human-readable format and
 * sent to the PC via UART.
 *
 */
void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel) {

    ADCBuf_adjustRawValues(handle, completedADCBuffer, ADCBUFFERSIZE, completedChannel);
    ADCBuf_convertAdjustedToMicroVolts(handle, completedChannel, completedADCBuffer, microVoltBuffer, ADCBUFFERSIZE);

    if (conversion->arg == &channelOne) {
        lastValue = microVoltBuffer[0];

        ADCBuf_convert(handle, &continuousConversionChannel[1], 1);
    } else {
        uint32_t difference = lastValue - microVoltBuffer[0];

        uartOutputBufferSize = System_sprintf(uartWriteBuffer, "%d,%d;", time++, difference);
        UART_write(uart, uartWriteBuffer, uartOutputBufferSize + 1);
        UART_read(uart, uartReadBuffer, UART_READ_BUFFER_SIZE);

        ADCBuf_convert(handle, &continuousConversionChannel[0], 1);
    }
}

void uartWriteCallback(UART_Handle handle, void *buf, size_t count) {
    return;
}

void uartReadCallback(UART_Handle handle, void *buf, size_t count) {
    int i;

    for (i = 0; i < UART_READ_BUFFER_SIZE; i++) {
        if (((char*) buf)[i] != endLineSequence[i]) {
            return;
        }
    }

    uartOutputBufferSize = System_sprintf(uartWriteBuffer, "\r\n");
    UART_write(handle, uartWriteBuffer, uartOutputBufferSize + 1);
}

/*
 *  ======== conversionStartFxn ========
 *  Task for this function is created statically. See the project's .cfg file.
 */
void conversionStartFxn(UArg arg0, UArg arg1) {
    UART_Params uartParams;
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.writeCallback = uartWriteCallback;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readCallback = uartReadCallback;
    uartParams.baudRate = 115200;
    uart = UART_open(Board_UART0, &uartParams);

    ADCBuf_Params_init(&adcBufParams);
    adcBufParams.callbackFxn = adcBufCallback;
    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_ONE_SHOT;
    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    adcBufParams.samplingFrequency = 1;
    adcBuf = ADCBuf_open(Board_ADCBuf0, &adcBufParams);

    /* Configure the conversion struct */
    continuousConversionChannel[0].arg = &channelOne;
    continuousConversionChannel[0].adcChannel = 9;
    continuousConversionChannel[0].sampleBuffer = sampleBufferCh1One;
    continuousConversionChannel[0].sampleBufferTwo = sampleBufferCh1Two;
    continuousConversionChannel[0].samplesRequestedCount = ADCBUFFERSIZE;

    continuousConversionChannel[1].arg = &channelTwo;
    continuousConversionChannel[1].adcChannel = 10;
    continuousConversionChannel[1].sampleBuffer = sampleBufferCh2One;
    continuousConversionChannel[1].sampleBufferTwo = sampleBufferCh2Two;
    continuousConversionChannel[1].samplesRequestedCount = ADCBUFFERSIZE;

    if (!adcBuf){
        System_abort("adcBuf did not open correctly\n");
    }

    /* Start converting. */
    if (ADCBuf_convert(adcBuf, continuousConversionChannel, 2) != ADCBuf_STATUS_SUCCESS) {
        System_abort("Did not start conversion process correctly\n");
    }

    /*
     * Go to sleep in the foreground thread forever. The data will be collected
     * and transfered in the background thread
     */
    Task_sleep(BIOS_WAIT_FOREVER);
}

/*
 *  ======== main ========
 */
int main(void) {
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initADCBuf();
    Board_initUART();

    /* Construct BIOS objects */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr) conversionStartFxn,
        &taskParams, NULL);

    System_printf("Starting the ADC Continuous Sampling example\n"
        "System provider is set to SysMin. Halt the target to view any SysMin "
        "contents in ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
