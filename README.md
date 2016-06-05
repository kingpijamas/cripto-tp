# cripto-tp

Para compilar: 
gcc main.c -o stegowav

Comandos validos:
./stegowav -embed -in ../resource/stitch.bmp -p ../resource/a.wav -out ../resource/a.wav -steg LSBE

./stegowav -embed -in ../resource/stitch.bmp -p ../resource/a.wav -out ../resource/a.wav -steg LSBE -a des -m cbc -pass "oculto"

./stegowav -extract -p ../resource/a.wav -out ../resource/stitch.bmp -steg LSBE -a des -m cbc -pass "oculto"