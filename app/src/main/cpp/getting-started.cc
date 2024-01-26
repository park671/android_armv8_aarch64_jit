// Copyright 2014, VIXL authors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "aarch64/macro-assembler-aarch64.h"

#include "getting-started.h"
#include "jit.h"

using namespace vixl;
using namespace vixl::aarch64;

void generateAddFunc(MacroAssembler *masm) {
    Label label;
    masm->Mov(x8, x0);
    masm->Mov(x9, x1);
    masm->Add(x0, x8, x9);
    masm->Ret();
    masm->Bl(&label);
    masm->Mov(x1, 3);
    masm->bind(&label);
    masm->Mov(x1, 2);
    masm->Mul(x0, x0, x1);
    masm->Ret();
}

extern "C" int main() {
    MacroAssembler masm;
    Label demo;
    masm.Bind(&demo);
    generateAddFunc(&masm);
    masm.FinalizeCode();
    byte *code = masm.GetBuffer()->GetStartAddress<byte *>();
    size_t code_size = masm.GetSizeOfCodeGenerated();
    int result = func(code, code_size, 4, 7);
    printf("add result=%d\n", result);
    return 0;
}
