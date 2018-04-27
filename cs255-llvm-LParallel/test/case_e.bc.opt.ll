; ModuleID = 'case_e.bc.opt'
source_filename = "<stdin>"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @test() #0 {
BB_:
  %c = alloca [100 x i32], align 16
  %n = alloca i32, align 4
  store i32 100, i32* %n, align 4
  br label %BB_1

BB_1:                                             ; preds = %BB_5, %BB_
  %0 = load i32, i32* %n, align 4
  %1 = icmp sgt i32 %0, 0
  br i1 %1, label %BB_2, label %BB_6

BB_2:                                             ; preds = %BB_1
  %2 = load i32, i32* %n, align 4
  %3 = icmp sgt i32 %2, 50
  br i1 %3, label %BB_3, label %BB_4

BB_3:                                             ; preds = %BB_2
  %4 = load i32, i32* %n, align 4
  %5 = add nsw i32 %4, 1
  %6 = sext i32 %5 to i64
  %7 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %6
  %a0 = load i32, i32* %7, align 4
  %8 = load i32, i32* %n, align 4
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %9
  %a1 = load i32, i32* %10, align 4
  %11 = add nsw i32 %a0, %a1
  %12 = load i32, i32* %n, align 4
  %13 = sext i32 %12 to i64
  %a2 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %13
  store i32 %11, i32* %a2, align 4
  br label %BB_5

BB_4:                                             ; preds = %BB_2
  %14 = load i32, i32* %n, align 4
  %15 = add nsw i32 %14, 1
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %16
  %a3 = load i32, i32* %17, align 4
  %18 = load i32, i32* %n, align 4
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %19
  %a4 = load i32, i32* %20, align 4
  %21 = sub nsw i32 %a3, %a4
  %22 = load i32, i32* %n, align 4
  %23 = sext i32 %22 to i64
  %a5 = getelementptr inbounds [100 x i32], [100 x i32]* %c, i64 0, i64 %23
  store i32 %21, i32* %a5, align 4
  br label %BB_5

BB_5:                                             ; preds = %BB_4, %BB_3
  %24 = load i32, i32* %n, align 4
  %25 = add nsw i32 %24, -1
  store i32 %25, i32* %n, align 4
  br label %BB_1

BB_6:                                             ; preds = %BB_1
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
