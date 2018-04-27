; Directed test cases for exp
;
; Copyright (c) 2018, Arm Limited.
; SPDX-License-Identifier: Apache-2.0
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

func=exp op1=7ff80000.00000001 result=7ff80000.00000001 errno=0
func=exp op1=fff80000.00000001 result=7ff80000.00000001 errno=0
func=exp op1=7ff00000.00000001 result=7ff80000.00000001 errno=0 status=i
func=exp op1=fff00000.00000001 result=7ff80000.00000001 errno=0 status=i
func=exp op1=7ff00000.00000000 result=7ff00000.00000000 errno=0
func=exp op1=fff00000.00000000 result=00000000.00000000 errno=0
func=exp op1=7fefffff.ffffffff result=7ff00000.00000000 errno=ERANGE status=ox
func=exp op1=ffefffff.ffffffff result=00000000.00000000 errno=ERANGE status=ux
func=exp op1=00000000.00000000 result=3ff00000.00000000 errno=0
func=exp op1=80000000.00000000 result=3ff00000.00000000 errno=0
func=exp op1=00000000.00000001 result=3ff00000.00000000 errno=0
func=exp op1=80000000.00000001 result=3ff00000.00000000 errno=0
func=exp op1=3c900000.00000000 result=3ff00000.00000000.400 errno=0
func=exp op1=bc900000.00000000 result=3fefffff.ffffffff.800 errno=0
func=exp op1=3fe00000.00000000 result=3ffa6129.8e1e069b.c97 errno=0
func=exp op1=bfe00000.00000000 result=3fe368b2.fc6f9609.fe8 errno=0
func=exp op1=3ff00000.00000000 result=4005bf0a.8b145769.535 errno=0
func=exp op1=bff00000.00000000 result=3fd78b56.362cef37.c6b errno=0
func=exp op1=40000000.00000000 result=401d8e64.b8d4ddad.cc3 errno=0
func=exp op1=c0000000.00000000 result=3fc152aa.a3bf81cb.9fe errno=0
func=exp op1=3ff12345.6789abcd result=40075955.c34718ed.6e3 errno=0
func=exp op1=40862e42.fefa39ef result=7fefffff.ffffff2a.1b1 errno=0
func=exp op1=40862e42.fefa39f0 result=7ff00000.00000000 errno=ERANGE status=ox
func=exp op1=c0874910.d52d3051 result=00000000.00000001 status=ux
func=exp op1=c0874910.d52d3052 result=00000000.00000000 errno=ERANGE status=ux
