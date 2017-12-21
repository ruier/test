REM ****************************************************
REM Wind River SBC PowerQUICCII - On Board Boot Flash
REM Hardware Reset Configuration Word Programming Script
REM ****************************************************
tf conf 41 00000000 3964 FE000000
tf erase
tf test
bf 480 fffe ffff
mml 480 14ffffff ffffffff 34ffffff ffffffff 02ffffff ffffffff 07ffffff ffffffff
go
dml fe000000
