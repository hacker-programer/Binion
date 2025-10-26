git init
git add .
git commit -m "Preparar release"

set /p TAG="Ingrese el nombre del tag (ej: v1.0.0): "
git tag -a %TAG% -m "Release %TAG%"

git remote add origin https://github.com/hacker-programer/Binion.git
git push -u origin master
git push origin %TAG%

pause
