; ModuleID = 'case_b.bc.opt'
source_filename = "<stdin>"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @test() #0 {
BB_:
  %a = alloca [100 x i32], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %j, align 4
  store i32 0, i32* %i, align 4
  br label %BB_1

BB_1:                                             ; preds = %BB_3, %BB_
  %0 = load i32, i32* %i, align 4
  %1 = icmp slt i32 %0, 100
  br i1 %1, label %BB_2, label %BB_4

BB_2:                                             ; preds = %BB_1
  %2 = load i32, i32* %i, align 4
  %3 = mul nsw i32 2, %2
  %4 = add nsw i32 %3, 1
  store i32 %4, i32* %j, align 4
  br label %BB_3

BB_3:                                             ; preds = %BB_2
  %5 = load i32, i32* %i, align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, i32* %i, align 4
  br label %BB_1

BB_4:                                             ; preds = %BB_1
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
