; ModuleID = 'case_d.bc.opt'
source_filename = "<stdin>"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @test() #0 {
BB_:
  %a = alloca [100 x i32], align 16
  %b = alloca [100 x i32], align 16
  %n = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 100, i32* %n, align 4
  store i32 98, i32* %n, align 4
  br label %BB_1

BB_1:                                             ; preds = %BB_3, %BB_
  %0 = load i32, i32* %n, align 4
  %1 = icmp sge i32 %0, 1
  br i1 %1, label %BB_2, label %BB_4

BB_2:                                             ; preds = %BB_1
  %2 = load i32, i32* %n, align 4
  %3 = sub nsw i32 %2, 1
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %4
  %a0 = load i32, i32* %5, align 4
  %6 = load i32, i32* %n, align 4
  %7 = add nsw i32 %6, 1
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %8
  %a1 = load i32, i32* %9, align 4
  %10 = add nsw i32 %a0, %a1
  %11 = load i32, i32* %n, align 4
  %12 = sext i32 %11 to i64
  %a2 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %12
  store i32 %10, i32* %a2, align 4
  br label %BB_3

BB_3:                                             ; preds = %BB_2
  %13 = load i32, i32* %n, align 4
  %14 = add nsw i32 %13, -1
  store i32 %14, i32* %n, align 4
  br label %BB_1

BB_4:                                             ; preds = %BB_1
  store i32 1, i32* %x, align 4
  store i32 1, i32* %x, align 4
  br label %BB_5

BB_5:                                             ; preds = %BB_7, %BB_4
  %15 = load i32, i32* %x, align 4
  %16 = icmp slt i32 %15, 100
  br i1 %16, label %BB_6, label %BB_8

BB_6:                                             ; preds = %BB_5
  %17 = getelementptr inbounds [100 x i32], [100 x i32]* %b, i64 0, i64 100
  %a3 = load i32, i32* %17, align 16
  %18 = load i32, i32* %x, align 4
  %19 = sub nsw i32 %18, 1
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %20
  %a4 = load i32, i32* %21, align 4
  %22 = sub nsw i32 %a3, %a4
  %23 = load i32, i32* %x, align 4
  %24 = sext i32 %23 to i64
  %a5 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %24
  store i32 %22, i32* %a5, align 4
  br label %BB_7

BB_7:                                             ; preds = %BB_6
  %25 = load i32, i32* %x, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, i32* %x, align 4
  br label %BB_5

BB_8:                                             ; preds = %BB_5
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
