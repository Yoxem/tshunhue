; ModuleID = 'a.cpp'

; Function Attrs: norecurse nounwind uwtable
define i32 @main() {
  %1 = alloca i32, align 4 ; 可刪；align 4 為「記憶體位址指定為4的倍數」 int %i
  %a = alloca i32, align 4 ; int %a
  %b = alloca i32, align 4 ; int %b
  store i32 0, i32* %1, align 4 ; %a = 0；可刪
  store i32 12, i32* %a, align 4; %b = 12
  %2 = load i32, i32* %a, align 4; $2 = %a
  %3 = load i32, i32* %a, align 4; $3 = %b
  %4 = mul nsw i32 %3, 3; $4 = mul %3, 3
  %5 = add nsw i32 %2, %4 ; %5 = add %2, %4 nsw = 帶符號運算後如果溢位，會給出 poison value
  store i32 %5, i32* %b, align 4 ; %b = %5
  %6 = load i32, i32* %b, align 4 ; %6 = %b
  ret i32 %6
}
