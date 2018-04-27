; ModuleID = 'case_a.bc.opt'
source_filename = "<stdin>"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @test() #0 {
BB_:
  %a = alloca [100 x i32], align 16
  %temp = alloca i32, align 4
  %last = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i, align 4
  br label %BB_1

BB_1:                                             ; preds = %BB_3, %BB_
  %0 = load i32, i32* %i, align 4
  %1 = icmp slt i32 %0, 100
  br i1 %1, label %BB_2, label %BB_4

BB_2:                                             ; preds = %BB_1
  %2 = load i32, i32* %i, align 4
  %3 = sub nsw i32 100, %2
  store i32 %3, i32* %last, align 4
  %4 = load i32, i32* %i, align 4
  %5 = sext i32 %4 to i64
  %6 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %5
  %a0 = load i32, i32* %6, align 4
  store i32 %a0, i32* %temp, align 4
  %7 = load i32, i32* %last, align 4
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %8
  %a1 = load i32, i32* %9, align 4
  %10 = load i32, i32* %i, align 4
  %11 = sext i32 %10 to i64
  %a2 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %11
  store i32 %a1, i32* %a2, align 4
  %12 = load i32, i32* %temp, align 4
  %13 = load i32, i32* %last, align 4
  %14 = sext i32 %13 to i64
  %a3 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %14
  store i32 %12, i32* %a3, align 4
  br label %BB_3

BB_3:                                             ; preds = %BB_2
  %15 = load i32, i32* %i, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, i32* %i, align 4
  br label %BB_1

BB_4:                                             ; preds = %BB_1
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
