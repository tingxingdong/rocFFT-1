/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/

#pragma once

#ifndef  BUTTERFLY_CONSTANT_H
#define  BUTTERFLY_CONSTANT_H

//butterfly radix-3 constants
#define C3QA 0.50000000000000000000000000000000
#define C3QB 0.86602540378443864676372317075294

//butterfly radix-5 constants
#define C5QA 0.30901699437494742410229341718282
#define C5QB 0.95105651629515357211643933337938
#define C5QC 0.50000000000000000000000000000000
#define C5QD 0.58778525229247312916870595463907
#define C5QE 0.80901699437494742410229341718282

//butterfly radix-7 constants
#define C7Q1 -1.16666666666666651863693004997913
#define C7Q2  0.79015646852540022404554065360571
#define C7Q3  0.05585426728964774240049351305970
#define C7Q4  0.73430220123575240531721419756650
#define C7Q5  0.44095855184409837868031445395900
#define C7Q6  0.34087293062393136944265847887436
#define C7Q7 -0.53396936033772524066165487965918
#define C7Q8  0.87484229096165666561546458979137

//butterfly radix-8 constants
#define C8Q  0.70710678118654752440084436210485

//butterfly radix-11 constants
#define b11_0 0.9898214418809327
#define b11_1 0.9594929736144973
#define b11_2 0.9189859472289947
#define b11_3 0.8767688310025893
#define b11_4 0.8308300260037728
#define b11_5 0.7784344533346518
#define b11_6 0.7153703234534297
#define b11_7 0.6343562706824244
#define b11_8 0.3425847256816375
#define b11_9 0.5211085581132027

//butterfly radix-13 constants
#define b13_0  0.9682872443619840
#define b13_1  0.9578059925946651
#define b13_2  0.8755023024091479
#define b13_3  0.8660254037844386
#define b13_4  0.8595425350987748
#define b13_5  0.8534800018598239
#define b13_6  0.7693388175729806
#define b13_7  0.6865583707817543
#define b13_8  0.6122646503767565
#define b13_9  0.6004772719326652
#define b13_10 0.5817047785105157
#define b13_11 0.5751407294740031
#define b13_12 0.5220263851612750
#define b13_13 0.5200285718888646
#define b13_14 0.5165207806234897
#define b13_15 0.5149187780863157
#define b13_16 0.5035370328637666
#define b13_17 0.5000000000000000
#define b13_18 0.3027756377319946
#define b13_19 0.3014792600477098
#define b13_20 0.3004626062886657
#define b13_21 0.2517685164318833
#define b13_22 0.2261094450357824
#define b13_23 0.0833333333333333
#define b13_24 0.0386329546443481

//butterfly radix-16 constants
#define C16A 0.923879532511286738
#define C16B 0.382683432365089837

#endif //  BUTTERFLY_CONSTANT_H
