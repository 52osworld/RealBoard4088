for /f "delims=" %%a in ('dir /ad/s/b') do copy "MakeMdkProj.bat" "%%~a\" /y
cmd