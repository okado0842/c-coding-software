-ソースコードのコンパイル方法及び環境^
私の環境はwindows10 x64 Home です

freeglutを入れていないとコンパイルできないため、freeglut-MinGW-3.0.0-1.mp.zip
を解凍し、freeglut-MinGW-3.0.0-1.mp.zip\freeglut下のファイルおよびフォルダーを自身の都合のいい場所に移動させて下さい。
コンパイル時はwindows標準のコマンドプロンプト(cmd.exe)を用いてください。
下記のコンパイル方法はwindows 64bit向けのコンパイル方法です。
32bit でコンパイルしないでください。

こちらにfreeglutに関する詳しい情報が載っています。
http://freeglut.sourceforge.net/index.php

まずcdコマンドでソースコードがおいてあるディレクトリを参照します。
そして、コンパイルですが以下の二つを実行します
gcc -c -o test.o source.c -D FREEGLUT_STATIC -I"(freeglutのファイル群が置いてあるディレクトリ)\include"
gcc -o test.exe test.o -L"(freeglutのファイル群が置いてあるディレクトリ)\lib\x64" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows


私の場合C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64下にfreeglutの中身
を置いているため
gcc -c -o test.o source.c -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include"
gcc -o test.exe test.o -L"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib\x64" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows
です

ソフトウェアを動作させる場合、windows 64bit homeで解像度が1280*640のアプリケーションを
読み込めるコンピューターで行ってください、
Linux及びmac用にコンパイルしていないため動作はしないと思います。
windows 32bitやproなどの動作の確認は行っていません。