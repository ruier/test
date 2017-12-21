REM ****************************************************
REM Wind River SBC PowerQUICCII - SIMM Boot Flash
REM Hardware Reset Configuration Word Programming Script
REM ****************************************************
tf conf 166 00000000 3964 FE000000
tf erase
tf test
bf 6a0 fffe ffff
mml 6a0 1Cffffff ffffffff 34ffffff ffffffff 02ffffff ffffffff 07ffffff ffffffff
go
dml fe000000
