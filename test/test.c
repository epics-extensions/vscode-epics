epicsInt8
epicsUInt8
epicsInt16
epicsUInt16
epicsEnum16
epicsInt32
epicsUInt32
epicsInt64
epicsUInt64
epicsFloat32
epicsFloat64
epicsString

// C.E.A. IRFU/SIS/LDISC
//
// signalProcessingLib.c
//

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <epicsTypes.h>

typedef enum { false = 0, true = !false } bool;

static int signalProcessPrim(aSubRecord *precord) {
    
    epicsFloat32 *in = (epicsFloat32 *)precord->a;      // input signal waveform
    int noa = precord->noa;                             // input signal max size
    int nsamp = *(epicsUInt32 *)precord->b;             // acquisition number of samples
    int compFactor = *(epicsUInt32 *)precord->c;        // compression factor
    int deciFactor = *(epicsUInt32 *)precord->d;        // acquisition decimation factor
    float samplingRate = *(epicsFloat32 *)precord->e;   // sampling rate
    float highTreshold = *(epicsFloat32 *)precord->f;   // high threshold
    float lowTreshold = *(epicsFloat32 *)precord->g;    // low threshold
    int trigCount = *(epicsUInt32 *)precord->h;         // trigger counter
    int insize = floor((float)nsamp/deciFactor);        // input signal size
    
    epicsFloat32 *out = (epicsFloat32 *)precord->vala;                // output signal waveform
    int nova = precord->nova;                           // output signal max size
    int outsize = ceil((float)insize/compFactor);       // output signal size

    float avg = 0;         // pulse average
    float min = 1000000;   // signal min
    float max = -1000000;  // signal max
    float pmin = 1000000;  // pulse min
    float pmax = -1000000; // pulse max
    int i = 0;
    int j = 0;
    float val;

    // First sample of the pulse
    int firstSample = 0;
    // Last sample of the pulse
    int lastSample = 0;
    // Number of sample really in the pulse
    // (between lowTreashold and highTreshold and between left and right cursor)
    int nbSample = 0;
    // Count of ignoredSample inside the pulse
    int ignoredSample = 0;
    // Sub-count of ignoredSample
    int subIgnoredSample = 0;
    // Flag to detect the first point in the pulse
    bool firstPoint = true;
    // Average of the N last points where N = compFactor
    float avgSignal = 0;
    
    if ((insize == 0) || (insize > noa)) {
        printf("signalProcessPrim: %s: input signal size error: insize=%d\n", precord->name, insize);
        return 1; // don't process OUT links
    } else if ((outsize == 0) || (outsize > nova)) {
        printf("signalProcessPrim: %s: output signal size error: outsize=%d\n", precord->name, outsize);
        return 1; // don't process OUT links
    }

//    printf("signalProcessPrim: noa=%d nea=%d insize=%d compFactor=%d deciFactor=%d outsize=%d\n",
//            precord->noa, precord->nea, insize, compFactor, deciFactor, outsize);
    
    for (i=0; i<insize; i++) {
    
        val = *in++;
        avgSignal = avgSignal + val;
        if (val < min) min = val;
        if (val > max) max = val;
        
        if ((i+1) % compFactor == 0) {
            // compress samples
            *out++ = avgSignal / compFactor;
            avgSignal = 0;
            j++;
        }
        else if ((i+1) == insize) {
            // compress last samples
            *out++ = avgSignal / ((i+1) % compFactor);
            avgSignal = 0;
            j++;
        }

        if ((val >= lowTreshold) && (val <= highTreshold)) {
            // inside pulse & inside tresholds
            if (firstPoint) { firstSample = i; firstPoint = false; }
            if (val < pmin) pmin = val;
            if (val > pmax) pmax = val;
            avg = avg + val;
            nbSample++;
            lastSample = i;
            ignoredSample += subIgnoredSample;
            subIgnoredSample = 0;
        } else {
            // inside pulse & outside tresholds
            if (!firstPoint) subIgnoredSample++;
        }
    }
    
    if (nbSample > 0) {
        ((epicsFloat32 *)precord->valb)[0] = avg / nbSample; // pulse average
        ((epicsFloat32 *)precord->valc)[0] = pmin; // pulse min
        ((epicsFloat32 *)precord->vald)[0] = pmax; // pulse max
        ((epicsFloat32 *)precord->vale)[0] = avg * deciFactor; // pulse integral
        ((epicsFloat32 *)precord->valf)[0] = min; // signal min
        ((epicsFloat32 *)precord->valg)[0] = max; // signal max
        ((epicsFloat32 *)precord->valh)[0] = (lastSample - firstSample) * deciFactor * 1000 / samplingRate; // pulse width in ms
        ((epicsFloat32 *)precord->vali)[0] = (lastSample - firstSample - ignoredSample) * deciFactor * 1000 / samplingRate; // effective pulse width
    } else {
        ((epicsFloat32 *)precord->valb)[0] = 0.;
        ((epicsFloat32 *)precord->valc)[0] = 0.;
        ((epicsFloat32 *)precord->vald)[0] = 0.;
        ((epicsFloat32 *)precord->vale)[0] = 0.;
        ((epicsFloat32 *)precord->valf)[0] = 0.;
        ((epicsFloat32 *)precord->valg)[0] = 0.;
        ((epicsFloat32 *)precord->valh)[0] = 0.;
        ((epicsFloat32 *)precord->vali)[0] = 0.;
    }
    
    ((epicsFloat32 *)precord->valj)[0] = firstSample * deciFactor * 1000 / samplingRate; // left cursor
    ((epicsFloat32 *)precord->valk)[0] = lastSample * deciFactor * 1000 / samplingRate;  // right cursor
    ((epicsUInt32 *)precord->vall)[0] = ++trigCount; // trigger counter
    precord->neva = outsize; // number of output array elements
    
//    printf("signalProcessPrim: firstSample=%d lastSample=%d lcurs=%f rcurs=%f\n",
//            firstSample, lastSample, ((float *)precord->valj)[0], ((float *)precord->valk)[0]);
    
    return 0;
}

epicsRegisterFunction(signalProcessPrim);



static int signalProcessSecond(aSubRecord *precord) {
    
    epicsFloat32 *in = (epicsFloat32 *)precord->a;                  // input signal waveform
    int noa = precord->noa;                                  // input signal max size
    int nsamp = *(epicsUInt32 *)precord->b;                  // acquisition number of samples
    int compFactor = *(epicsUInt32 *)precord->c;             // compression factor
    int deciFactor = *(epicsUInt32 *)precord->d;             // acquisition decimation factor
    float samplingRate = *(epicsFloat32 *)precord->e;        // sampling rate
    float highTreshold = *(epicsFloat32 *)precord->f;        // high threshold
    float lowTreshold = *(epicsFloat32 *)precord->g;         // low threshold
    float lcurs = *(epicsFloat32 *)precord->i;               // left cursor
    float rcurs = *(epicsFloat32 *)precord->j;               // right cursor
    int cursor = *(epicsUInt16*)precord->k;                  // use cursors option
    int compress = *(epicsUInt16*)precord->l;                // output compressed signal waveform option
    int threshold = *(epicsUInt16*)precord->m;               // use thresholds option
    int insize = floor((float)nsamp/deciFactor);             // input signal size
    
    epicsFloat32 *out = (epicsFloat32 *)precord->vala;       // output signal waveform
    int nova = precord->nova;                                // output signal max size
    int outsize = ceil((float)insize/compFactor);            // output signal size

    float avg = 0;         // pulse average
    float min = 1000000;   // signal min
    float max = -1000000;  // signal max
    float pmin = 1000000;  // pulse min
    float pmax = -1000000; // pulse max
    int firstSample = 0;         // First sample of the pulse
    int lastSample = insize - 1; // last sample of the pulse
    int i = 0;
    int j = 0;
    float val;

    if (cursor) {
        firstSample = (lcurs * samplingRate) / (deciFactor * 1000);
        lastSample = (rcurs * samplingRate) / (deciFactor * 1000);
    }
    
    if (!threshold) {
        highTreshold = 1000000;
        lowTreshold = -1000000;
    }
    
    // Number of sample really in the pulse
    // (between lowTreashold and highTreshold and between left and right cursor)
    int nbSample = 0;
    // Average of the N last points where N = compFactor
    float avgSignal = 0;
    
    if ((insize == 0) || (insize > noa)) {
        printf("signalProcessSecond: %s: input signal size error: insize=%d\n", precord->name, insize);
        return 1; // don't process OUT links
    } else if ((outsize == 0) || (outsize > nova)) {
        printf("signalProcessSecond: %s: output signal size error: outsize=%d\n", precord->name, outsize);
        return 1; // don't process OUT links
    }

//    printf("signalProcessPrim: noa=%d nea=%d insize=%d compFactor=%d deciFactor=%d outsize=%d\n",
//            precord->noa, precord->nea, insize, compFactor, deciFactor, outsize);
    
    for (i = 0; i < insize; i++) {
    
        val = *in++;
        avgSignal = avgSignal + val;
        if (val < min) min = val;
        if (val > max) max = val;
        
        // output compressed signal waveform
        if (compress) {
            if ((i+1) % compFactor == 0) {
                // compress samples
                *out++ = avgSignal / compFactor;
                avgSignal = 0;
                j++;
            }
            else if ((i+1) == insize) {
                // compress last samples
                *out++ = avgSignal / ((i+1) % compFactor);
                avgSignal = 0;
                j++;
            }
        }
        
        if ((val >= lowTreshold) &&
            (val <= highTreshold) &&
            (i >= firstSample) &&
            (i <= lastSample)) {
            if (val < pmin) pmin = val;
            if (val > pmax) pmax = val;
                avg = avg + val;
                nbSample++;
        }
    }
    
    if (nbSample > 0) {
        ((epicsFloat32 *)precord->valb)[0] = avg / nbSample;   // pulse average
        ((epicsFloat32 *)precord->valc)[0] = pmin;             // pulse min
        ((epicsFloat32 *)precord->vald)[0] = pmax;             // pulse max
        ((epicsFloat32 *)precord->vale)[0] = avg * deciFactor; // pulse integral
        ((epicsFloat32 *)precord->valf)[0] = min;              // signal min
        ((epicsFloat32 *)precord->valg)[0] = max;              // signal max
    } else {
        ((epicsFloat32 *)precord->valb)[0] = 0.;
        ((epicsFloat32 *)precord->valc)[0] = 0.;
        ((epicsFloat32 *)precord->vald)[0] = 0.;
        ((epicsFloat32 *)precord->vale)[0] = 0.;
        ((epicsFloat32 *)precord->valf)[0] = 0.;
        ((epicsFloat32 *)precord->valg)[0] = 0.;
    }
    
    if (compress)
        precord->neva = outsize; // number of output array elements
    else
        precord->neva = 0;
    
    return 0;
}

epicsRegisterFunction(signalProcessSecond);



static int signalProcessCursor(aSubRecord *precord) {
    
    float x  = *(epicsFloat32 *)precord->a;
    float y0 = *(epicsFloat32 *)precord->b;
    float y1 = *(epicsFloat32 *)precord->c;

    ((epicsFloat32 *)precord->vala)[0] = x;
    ((epicsFloat32 *)precord->vala)[1] = x;
    ((epicsFloat32 *)precord->valb)[0] = y0;
    ((epicsFloat32 *)precord->valb)[1] = y1;
    
//    printf("signalProcessCursor: %s: x=%f y0=%f y1=%f\n", precord->name, x, y0, y1);
    
    return 0;
}

epicsRegisterFunction(signalProcessCursor);


static int signalProcessXAxis(aSubRecord *precord) {
    
    float samplingTime = *(epicsFloat32 *)precord->a;   // sampling time
    int size = *(epicsUInt32 *)precord->b;              // axis size
    epicsFloat32 *out = (epicsFloat32 *)precord->vala;  // output axis
    int nova = precord->nova;                           // axis max size
    float delta = 0;
    int i;
        
    if ((size == 0) || (size > nova)) {
        printf("signalProcessXAxis: %s: axis size error: size=%d\n", precord->name, size);
        return 1; // don't process OUT links
    } else {
        delta = samplingTime / size;
        printf("signalProcessXAxis: %s: samplingTime=%fms size=%d delta=%fms\n", precord->name, samplingTime, size, delta);
        for (i=0; i<size; i++) *out++ = delta * i;
        precord->neva = size; // number of output array elements
        return 0;
    }
}

epicsRegisterFunction(signalProcessXAxis);

