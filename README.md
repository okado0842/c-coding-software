-�\�[�X�R�[�h�̃R���p�C�����@�y�ъ�^
���̊���windows10 x64 Home �ł�

freeglut�����Ă��Ȃ��ƃR���p�C���ł��Ȃ����߁Afreeglut-MinGW-3.0.0-1.mp.zip
���𓀂��Afreeglut-MinGW-3.0.0-1.mp.zip\freeglut���̃t�@�C������уt�H���_�[�����g�̓s���̂����ꏊ�Ɉړ������ĉ������B
�R���p�C������windows�W���̃R�}���h�v�����v�g(cmd.exe)��p���Ă��������B
���L�̃R���p�C�����@��windows 64bit�����̃R���p�C�����@�ł��B
32bit �ŃR���p�C�����Ȃ��ł��������B

�������freeglut�Ɋւ���ڂ�����񂪍ڂ��Ă��܂��B
http://freeglut.sourceforge.net/index.php

�܂�cd�R�}���h�Ń\�[�X�R�[�h�������Ă���f�B���N�g�����Q�Ƃ��܂��B
�����āA�R���p�C���ł����ȉ��̓�����s���܂�
gcc -c -o test.o source.c -D FREEGLUT_STATIC -I"(freeglut�̃t�@�C���Q���u���Ă���f�B���N�g��)\include"
gcc -o test.exe test.o -L"(freeglut�̃t�@�C���Q���u���Ă���f�B���N�g��)\lib\x64" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows


���̏ꍇC:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64����freeglut�̒��g
��u���Ă��邽��
gcc -c -o test.o source.c -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include"
gcc -o test.exe test.o -L"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib\x64" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows
�ł�

�\�t�g�E�F�A�𓮍삳����ꍇ�Awindows 64bit home�ŉ𑜓x��1280*640�̃A�v���P�[�V������
�ǂݍ��߂�R���s���[�^�[�ōs���Ă��������A
Linux�y��mac�p�ɃR���p�C�����Ă��Ȃ����ߓ���͂��Ȃ��Ǝv���܂��B
windows 32bit��pro�Ȃǂ̓���̊m�F�͍s���Ă��܂���B