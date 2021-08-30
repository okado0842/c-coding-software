#include<stdio.h>
#include<string.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
char window_name[5]="test3",block_name[9],text[999][3][30],cpy_text[999][3][30];
unsigned char typing_p=0;//タイピングの現在位置
char temp_text[9];//一時的に使うための文字列
unsigned char page=0;//現在のページ 0=編集 1=宣言 2=ブロックの中身の記述
float windowsize_w=1280.0,windowsize_h=640.0;
float mouse_x,mouse_y;
int act[6],block_d;
unsigned char current_abc=0;//現在選択している変数の情報
int abc[58];//変数の型
char abc2[58][9];//変数の配列情報
int i,j,k,block[999],bp_a,block_p[999],key_s,cpy_block[999];
int select_addr[2];//[0]切り取るブロックの内一番上のブロックの住所,[1]一番下の住所
float bdp[2];//ブロックを描画する際のポジション
FILE *fpb;

void hairetsu_nakami(int temp_a){
int temp_b;
if(text[i][j][temp_a]<'A'||text[i][j][temp_a]>'z'){//変数ではなく数値を扱う場合

for(temp_b=0;temp_b<8;temp_b++){//数字を記入する作業
if((text[i][j][temp_a+temp_b+1]>='0'&&text[i][j][temp_a+temp_b+1]<='9')||text[i][j][temp_a+temp_b+1]=='.'){
//数字もしくは小数点じゃないものを省く
fprintf(fpb,"%c",text[i][j][temp_a+temp_b+1]);
}else{printf("wrong string");}//end if
}//for



}else{//変数を扱う場合

fprintf(fpb,"%c",text[i][j][temp_a]);
if(text[i][j][temp_a+1]!=0)//配列数が記入されている時　すなわち配列を持つとき
{
fprintf(fpb,"[");
if(text[i][j][temp_a+1]>='A'&&text[i][j][temp_a+1]<='z'){//配列番号が変数の場合
fprintf(fpb,"%c",text[i][j][temp_a+1]);
}else{//配列番号が整数の場合
for(temp_b=0;temp_b<8;temp_b++){//配列番号を記入する作業
if(text[i][j][temp_a+temp_b+1]>='0'&&text[i][j][temp_a+temp_b+1]<='9'){//数字じゃないものを省く
fprintf(fpb,"%c",text[i][j][temp_a+temp_b+1]);
}else{printf("wrong string");}//end if
}//for
}//if
fprintf(fpb,"]");

}//if

}//if

}//func


int kind_of_abc(int temp_a){
if(abc[text[i][j][temp_a]-'A']==1){return 1;}
if(abc[text[i][j][temp_a]-'A']==2){return 2;}
if(abc[text[i][j][temp_a]-'A']==3){
if(text[i][j][temp_a+1]==0&&(abc2[text[i][j][temp_a]-'A'][0]!=0)){
return 4;}else{return 3;}
}//if
return 0;
}

void translating_f(){
int temp_b;
for(temp_b=0;temp_b<8;temp_b++){//数値を記入する作業
if(text[i][j][temp_b]>='0'&&text[i][j][temp_b]<='9'){//数字じゃないものを省く
fprintf(fpb,"%c",text[i][j][temp_b]);}
}
}//func

void translating_e(){
hairetsu_nakami(0);
}

void translating_c(){
int temp_b;
fprintf(fpb,"\"");
switch(kind_of_abc(1)){
case 0:
for(temp_b=0;temp_b<8;temp_b++){//記述を記入する作業
if(text[i][j][temp_b+2]!=0){
fprintf(fpb,"%c",text[i][j][temp_b+2]);}
}
fprintf(fpb,"\"");
return;
case 1:fprintf(fpb,"%%d");break;
case 2:fprintf(fpb,"%%f");break;
case 3:fprintf(fpb,"%%c");break;
case 4:fprintf(fpb,"%%s");break;
}//switch
fprintf(fpb,"\"");
fprintf(fpb,",");
switch(kind_of_abc(1)){
case 4:fprintf(fpb,"%c",text[i][j][1]);break;
default:hairetsu_nakami(1);break;
}//switch
}//func

void translating_d(){
int temp_b;
fprintf(fpb,"\"");
switch(kind_of_abc(1)){
case 0:
fprintf(fpb,"error");
fprintf(fpb,"\"");
return;
case 1:fprintf(fpb,"%%d");break;
case 2:fprintf(fpb,"%%f");break;
case 3:fprintf(fpb,"%%c");break;
case 4:fprintf(fpb,"%%s");break;
}//switch
fprintf(fpb,"\"");
fprintf(fpb,",");
switch(kind_of_abc(1)){
case 4:fprintf(fpb,"%c",text[i][j][1]);break;
default:fprintf(fpb,"&");hairetsu_nakami(1);break;
}//switch
}//func






void translating_b(){
hairetsu_nakami(0);
switch(text[i][j][9]){
case '0':fprintf(fpb,"==");break;
case '1':fprintf(fpb,"!=");break;
case '2':fprintf(fpb,"<");break;
case '3':fprintf(fpb,"<");break;
case '4':fprintf(fpb,">=");break;
case '5':fprintf(fpb,"<=");break;
default :fprintf(fpb,"==");break;
}//switch
hairetsu_nakami(10);
switch(text[i][j][19]){
case '0':fprintf(fpb,"+");break;
case '1':fprintf(fpb,"-");break;
case '2':fprintf(fpb,"*");break;
case '3':fprintf(fpb,"/");break;
case '4':fprintf(fpb,"%");break;
default :fprintf(fpb,"+");break;
}//switch
hairetsu_nakami(20);
}//func

void translating_a(){
hairetsu_nakami(0);//変数を置く
fprintf(fpb,"=");//等式を置く
hairetsu_nakami(9);
switch(text[i][j][18]){
case '0':fprintf(fpb,"+");break;
case '1':fprintf(fpb,"-");break;
case '2':fprintf(fpb,"*");break;
case '3':fprintf(fpb,"/");break;
case '4':fprintf(fpb,"%");break;
default :fprintf(fpb,"+");break;
}//switch
hairetsu_nakami(19);
}//func





void translating(){
fpb=fopen("program.c","w");
fprintf(fpb,"#include<stdio.h>\n");
for(i=0;i<58;i++){
switch(abc[i]){
case 1:fprintf(fpb,"int ");break;
case 2:fprintf(fpb,"float ");break;
case 3:fprintf(fpb,"char ");break;
default:continue;
}//switch
fprintf(fpb,"%c",i+'A');
if(abc2[i][0]!=0){
fprintf(fpb,"[");
fprintf(fpb,"%s",abc2[i]);
fprintf(fpb,"]");
}//if
fprintf(fpb,";\n");
}//for

fprintf(fpb,"void main(){\n\n");

for(i=0;i<999;i++){
for(j=0;j<block_p[i];j++){//空白の積み上げ
fprintf(fpb,"    ");
}//for
switch(block[i]){
case 10100:fprintf(fpb,"}\n");break;
case 10501:fprintf(fpb,"if(");j=0;translating_b();fprintf(fpb,"){\n");break;
case 10502:fprintf(fpb,"}else{\n");break;
case 10503:fprintf(fpb,"}else if(");j=0;translating_b();fprintf(fpb,"){\n");break;
case 10200:fprintf(fpb,"for(");j=0;translating_a();fprintf(fpb,";");j=1;translating_b();fprintf(fpb,";");j=2;translating_a();fprintf(fpb,"){\n");break;
case 11900:fprintf(fpb,"while(");j=0;translating_b();fprintf(fpb,"){\n");break;
case 11500:fprintf(fpb,"switch(");j=0;translating_e();fprintf(fpb,"){\n");break;
case 11200:fprintf(fpb,"printf(");j=0;translating_c();fprintf(fpb,");\n");break;
case 11501:fprintf(fpb,"scanf(");j=0;translating_d();fprintf(fpb,");\n");break;
case 9900:fprintf(fpb,"case ");j=0;translating_f();fprintf(fpb,":\n");break;
case 10000:fprintf(fpb,"default:\n");break;
case 9800:fprintf(fpb,"break;\n");break;
case 10900:j=0;translating_a();fprintf(fpb,";\n");
default:break;
}//switch
}//for
fprintf(fpb,"\n}//end of main func");
fclose(fpb);
}












void delete_blocks(){
for(i=0;i<999;i++){
block[i]=0;
block_p[i]=0;
for(j=0;j<3;j++){
for(k=0;k<29;k++){
text[i][j][k]=0;
}//for k
}//for j
}//for i
}//func








void open_file(){

FILE *fpa;
FILE *fpc;
fpa=fopen("save.txt","r");
fpc=fopen("save2.txt","r");
for(i=0;i<999;i++){
fscanf(fpa,"%d",&block[i]);
}
for(i=0;i<999;i++){
for(i=0;i<3;i++){
fscanf(fpa,"%s",text[i][j]);
}//for
}//for
for(i=0;i<58;i++){
fscanf(fpc,"%d",&abc[i]);
}
for(i=0;i<58;i++){
for(i=0;i<8;i++){
fscanf(fpc,"%s",abc2[i][j]);
}
}
fclose(fpa);
fclose(fpc);
}

void save_file(){

FILE *fpa;
FILE *fpc;
fpa=fopen("save.txt","w");
fpc=fopen("save2.txt","w");
for(i=0;i<999;i++){
fprintf(fpa,"%d",block[i]);
fprintf(fpa,"\n");
}
for(i=0;i<999;i++){
for(i=0;i<3;i++){
fprintf(fpa,"%s",text[i][j]);
fprintf(fpa,"\n");
}//for
}//for
for(i=0;i<58;i++){
fprintf(fpc,"%d",abc[i]);
fprintf(fpa,"\n");
}
for(i=0;i<58;i++){
for(i=0;i<8;i++){
fprintf(fpc,"%s",abc2[i][j]);
fprintf(fpa,"\n");
}
}
fclose(fpa);
fclose(fpc);
}



void block_positioning(){//ブロックの順番が並び替えられるたびに実行
bp_a=0;
for(i=0;i<999;i++){

switch(block[i]){
case 10100:bp_a--; block_p[i]=bp_a; break;
case 10501:block_p[i]=bp_a;bp_a++; break;
case 10502:bp_a--; block_p[i]=bp_a; bp_a++; break;
case 10503:bp_a--; block_p[i]=bp_a; bp_a++; break;
case 10200:block_p[i]=bp_a;bp_a++; break;
case 11900:block_p[i]=bp_a;bp_a++; break;
case 11500:block_p[i]=bp_a;bp_a++; break;
case 9900:bp_a--; block_p[i]=bp_a; bp_a++; break;
case 10000:bp_a--; block_p[i]=bp_a; bp_a++; break;
default:block_p[i]=bp_a; break;
}//switch
}//for

}//func

void RenderString(float x,float y,void *font,const char *string)//テキストの出力
{
glRasterPos2f(x,y);
glutBitmapString(font,string);
}

void draw_blocks_a(){//ブロックの描画
glBegin(GL_POLYGON);
glVertex2f(bdp[0],bdp[1]);
glVertex2f(bdp[0]+0.1,bdp[1]);
glVertex2f(bdp[0]+0.1,bdp[1]-0.05);
glVertex2f(bdp[0],bdp[1]-0.05);
glEnd();
glColor3f(1.0,1.0,1.0);
RenderString(bdp[0],bdp[1]-0.0375,GLUT_BITMAP_TIMES_ROMAN_24,block_name);
}

void draw_blocks()//編集画面上のブロックの描画
{
for(i=block_d;i<block_d+16;i++){//block_dは画面上の一番上にあるブロックが　全体において何番目の数か。iは全体からi番目の数/-block_dで画面上における順番　のこと
bdp[0]=-0.5 +block_p[i]*0.05 - 0.05*act[4];//i番目のブロックのx座標の決定
bdp[1]=0.4 -i*0.05 +block_d*0.05;//i番目のブロックのy座標の決定
switch(block[i]){
case 10100:glColor3f(0.0,0.0,0.0);strcpy(block_name,"end");draw_blocks_a();break;//色と表示するテキストの決定
case 10501:glColor3f(0.0,1.0,1.0);strcpy(block_name,"if");draw_blocks_a();break;
case 10502:glColor3f(0.0,1.0,1.0);strcpy(block_name,"else");draw_blocks_a();break;
case 10503:glColor3f(0.0,1.0,1.0);strcpy(block_name,"elseif");draw_blocks_a();break;
case 10200:glColor3f(0.0,0.5,0.0);strcpy(block_name,"for");draw_blocks_a();break;
case 11900:glColor3f(1.0,0.0,0.0);strcpy(block_name,"while");draw_blocks_a();break;
case 11901:glColor3f(1.0,0.0,0.0);strcpy(block_name,"while");draw_blocks_a();break;
case 11500:glColor3f(1.0,0.7,0.7);strcpy(block_name,"switch");draw_blocks_a();break;
case 11200:glColor3f(1.0,1.0,0.0);strcpy(block_name,"printf");draw_blocks_a();break;
case 11501:glColor3f(1.0,0.5,0.0);strcpy(block_name,"scanf");draw_blocks_a();break;
case 9900:glColor3f(1.0,0.5,0.5);strcpy(block_name,"case");draw_blocks_a();break;
case 9800:glColor3f(0.0,0.0,1.0);strcpy(block_name,"break");draw_blocks_a();break;
case 10000:glColor3f(1.0,0.5,0.5);strcpy(block_name,"default");draw_blocks_a();break;
case 10900:glColor3f(0.0,0.9,0.0);strcpy(block_name,"ABC");draw_blocks_a();break;
default :break;
}//switch
}//for
}//func

void draw_panels_a(){
bdp[0]=-0.65;
bdp[1]=0.4-i*0.05*1;
}
void draw_panels(){
i=0;
glColor3f(0.0,1.0,1.0);strcpy(block_name,"if");draw_panels_a();draw_blocks_a();i++;
glColor3f(0.0,1.0,1.0);strcpy(block_name,"elseif");draw_panels_a();draw_blocks_a();i++;
glColor3f(0.0,0.5,0.0);strcpy(block_name,"for");draw_panels_a();draw_blocks_a();i++;
glColor3f(1.0,0.0,0.0);strcpy(block_name,"while");draw_panels_a();draw_blocks_a();i++;
glColor3f(1.0,0.7,0.7);strcpy(block_name,"switch");draw_panels_a();draw_blocks_a();i++;
glColor3f(1.0,0.5,0.5);strcpy(block_name,"case");draw_panels_a();draw_blocks_a();i++;
glColor3f(0.0,0.0,1.0);strcpy(block_name,"break");draw_panels_a();draw_blocks_a();i++;
glColor3f(1.0,1.0,0.0);strcpy(block_name,"printf");draw_panels_a();draw_blocks_a();i++;
glColor3f(1.0,0.5,0.0);strcpy(block_name,"scanf");draw_panels_a();draw_blocks_a();i++;
glColor3f(0.0,0.9,0.0);strcpy(block_name,"ABC");draw_panels_a();draw_blocks_a();i++;
}

void draw_panels_c(){
bdp[0]=-0.95;
bdp[1]=0.4-i*0.05*1;
}


void draw_panels_b(){
i=0;
glColor3f(1.0,0.6,0.6);strcpy(block_name,"save");draw_panels_c();draw_blocks_a();i++;
glColor3f(1.0,0.0,1.0);strcpy(block_name,"open");draw_panels_c();draw_blocks_a();i++;
glColor3f(0.5,0.5,0.0);strcpy(block_name,"build");draw_panels_c();draw_blocks_a();i++;
glColor3f(0.6,0.0,0.0);strcpy(block_name,"delete");draw_panels_c();draw_blocks_a();i++;
}

void draw_moving_panels(){
float temp_a[2];
temp_a[0]=-0.75,temp_a[1]=0.0;
glColor3f(0.9,0.9,0.9);

/*glBegin(GL_POLYGON);
glVertex2f(temp_a[0],temp_a[1]);
glVertex2f(temp_a[0]+0.08,temp_a[1]);
glVertex2f(temp_a[0]+0.08,temp_a[1]+0.08);
glVertex2f(temp_a[0],temp_a[1]+0.08);
glEnd();
glBegin(GL_POLYGON);
glVertex2f(temp_a[0]-0.08,temp_a[1]);
glVertex2f(temp_a[0],temp_a[1]);
glVertex2f(temp_a[0],temp_a[1]+0.08);
glVertex2f(temp_a[0]-0.08,temp_a[1]+0.08);
glEnd();
glBegin(GL_POLYGON);
glVertex2f(temp_a[0],temp_a[1]-0.08);
glVertex2f(temp_a[0]+0.08,temp_a[1]-0.08);
glVertex2f(temp_a[0]+0.08,temp_a[1]);
glVertex2f(temp_a[0],temp_a[1]);
glEnd();
glBegin(GL_POLYGON);
glVertex2f(temp_a[0]-0.08,temp_a[1]-0.08);
glVertex2f(temp_a[0],temp_a[1]-0.08);
glVertex2f(temp_a[0],temp_a[1]);
glVertex2f(temp_a[0]-0.08,temp_a[1]);
glEnd();*/

glBegin(GL_POLYGON);
glVertex2f(temp_a[0]-0.08,temp_a[1]-0.08);
glVertex2f(temp_a[0]+0.08,temp_a[1]-0.08);
glVertex2f(temp_a[0]+0.08,temp_a[1]+0.08);
glVertex2f(temp_a[0]-0.08,temp_a[1]+0.08);
glEnd();
glColor3f(0.8,0.8,0.8);

glBegin(GL_POLYGON);//右上
glVertex2f(temp_a[0]+0.02,temp_a[1]+0.02);
glVertex2f(temp_a[0]+0.04,temp_a[1]+0.06);
glVertex2f(temp_a[0]+0.06,temp_a[1]+0.02);
glEnd();
glBegin(GL_POLYGON);//右下
glVertex2f(temp_a[0]+0.02,temp_a[1]-0.02);
glVertex2f(temp_a[0]+0.04,temp_a[1]-0.06);
glVertex2f(temp_a[0]+0.06,temp_a[1]-0.02);
glEnd();
glBegin(GL_POLYGON);//左上
glVertex2f(temp_a[0]-0.02,temp_a[1]+0.06);
glVertex2f(temp_a[0]-0.06,temp_a[1]+0.04);
glVertex2f(temp_a[0]-0.02,temp_a[1]+0.02);
glEnd();
glBegin(GL_POLYGON);//左下
glVertex2f(temp_a[0]-0.06,temp_a[1]-0.06);
glVertex2f(temp_a[0]-0.02,temp_a[1]-0.04);
glVertex2f(temp_a[0]-0.06,temp_a[1]-0.02);
glEnd();

glBegin(GL_LINES);//横
glVertex2f(temp_a[0]-0.08,temp_a[1]);
glVertex2f(temp_a[0]+0.08,temp_a[1]);
glEnd();
glBegin(GL_LINES);//縦
glVertex2f(temp_a[0],temp_a[1]-0.08);
glVertex2f(temp_a[0],temp_a[1]+0.08);
glEnd();

}

void draw_current_panel_a(){
glBegin(GL_LINE_LOOP);
glVertex2f(bdp[0],bdp[1]);
glVertex2f(bdp[0]+0.1,bdp[1]);
glVertex2f(bdp[0]+0.1,bdp[1]-0.05);
glVertex2f(bdp[0],bdp[1]-0.05);
glEnd();
}

void draw_current_panel_b(){
glBegin(GL_LINE_LOOP);
glVertex2f(bdp[0],bdp[1]);//bdp[]+?は点から点への長さ
glVertex2f(bdp[0]+0.08,bdp[1]);
glVertex2f(bdp[0]+0.08,bdp[1]+0.08);
glVertex2f(bdp[0],bdp[1]+0.08);
glEnd();
}

void draw_current_panel_c(){
glBegin(GL_LINE_LOOP);
glVertex2f(bdp[0],bdp[1]);
glVertex2f(bdp[0]+1.4,bdp[1]);
glVertex2f(bdp[0]+1.4,bdp[1]-0.05);
glVertex2f(bdp[0],bdp[1]-0.05);
glEnd();
}

void draw_current_panel(){
glColor3f(0.7,0.7,0.0);
float temp_a[2];//temp_aは画面移動パネルの中心座標
temp_a[0]=-0.75;temp_a[1]=0.0;
switch(act[0]){
case 0:bdp[0]=-0.95;bdp[1]=0.4-0.05*act[1];draw_current_panel_a();break;
case 1:bdp[0]=temp_a[0]-0.08;bdp[1]=temp_a[1]-0.08*act[1];draw_current_panel_b();break;
case 2:bdp[0]=temp_a[0];bdp[1]=temp_a[1]-0.08*act[1];draw_current_panel_b();break;
case 3:bdp[0]=-0.65;bdp[1]=0.4-0.05*act[1];draw_current_panel_a();break;
case 4:bdp[0]=-0.5;bdp[1]=0.4-0.05*act[1];draw_current_panel_c();break;
default:break;
}

}

void draw_text_box(float temp_a,float temp_b,int temp_c,int temp_d){//temp_aおよびbは座標　cは今何番目を参照しているかを指すdは何番目の引数か
glColor3f(1.0,1.0,1.0);
if(act[2]==temp_c&&act[3]==temp_d){glColor3f(1.0,0.8,1.0);}//選択されている場合枠は紫
glBegin(GL_POLYGON);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(1.0,1.0,0.0);
RenderString(temp_a-0.009375,temp_b-0.009375,GLUT_BITMAP_TIMES_ROMAN_24,temp_text);
}
void draw_text_strbox(float temp_a,float temp_b,int temp_c,int temp_d){
glColor3f(1.0,1.0,1.0);
if(act[2]==temp_c&&act[3]==temp_d){glColor3f(1.0,0.8,1.0);}//選択されている場合枠は紫
glBegin(GL_POLYGON);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(1.0,1.0,0.0);
RenderString(temp_a-0.209375,temp_b-0.009375,GLUT_BITMAP_TIMES_ROMAN_24,temp_text);
}

int block_type(int temp_a){//テキストのタイプの判断
if(block[temp_a]==10501||block[temp_a]==10503||block[temp_a]==11900){//条件式(単体)の場合
return 0;}
if(block[temp_a]==10900){//abc 代入式　の場合
return 1;}
if(block[temp_a]==10200){//for 代入式；条件式；代入式　の場合
return 2;}
if(block[temp_a]==11501||block[temp_a]==11200){//記述の場合
return 3;}
if(block[temp_a]==11500){//変数単体の場合
return 4;}
if(block[temp_a]==9900){//数値
return 5;}
return -1;
}//func

int text_type(){//ブロックの記述の中身の配列一つ一つが何についてのことか 1=変数 2=演算子 3=比較演算子 4=配列番号及び数値 5=記述 6=改行の有無
if(block_type(act[1]+block_d)==0||(block_type(act[1]+block_d)==2&&act[3]==1)){
switch(act[2]){
case 0:return 1;
case 1:return 4;
case 9:return 3;
case 10:return 1;
case 11:return 4;
case 19:return 2;
case 20:return 1;
case 21:return 4;
}//switch
}//if　条件式
if(block_type(act[1]+block_d)==1||(block_type(act[1]+block_d)==2&&act[3]!=1)){
switch(act[2]){
case 0:return 1;
case 1:return 4;
case 9:return 1;
case 10:return 4;
case 18:return 2;
case 19:return 1;
case 20:return 4;
}//switch
}//if　代入式　
if(block_type(act[1]+block_d)==3){
switch(act[2]){
case 0:return 6;
case 1:return 1;
case 2:return 4;
}//switch
}//if　記述
if(block_type(act[1]+block_d)==4){
switch(act[2]){
case 0:return 1;
case 1:return 4;
}//switch
}//if　変数のみ
if(block_type(act[1]+block_d==5)){
return 4;
}//if　数値
}//func




void func_drawing_text_1(int temp_b,int temp_a){
switch(text[act[1]+block_d][temp_a][temp_b]){
case '0':strcpy(temp_text,"==");break;
case '1':strcpy(temp_text,"!=");break;
case '2':strcpy(temp_text,"<");break;
case '3':strcpy(temp_text,">");break;
case '4':strcpy(temp_text,">=");break;
case '5':strcpy(temp_text,"<=");break;
default :strcpy(temp_text,"null");break;
}//switch 
}//func 比較演算子

void func_drawing_text_2(int temp_b,int temp_a){
switch(text[act[1]+block_d][temp_a][temp_b]){
case '0':strcpy(temp_text,"+");break;
case '1':strcpy(temp_text,"-");break;
case '2':strcpy(temp_text,"*");break;
case '3':strcpy(temp_text,"/");break;
case '4':strcpy(temp_text,"%");break;
default :strcpy(temp_text,"null");break;
}//switch 
}//func 演算子

void func_drawing_text_3(int temp_b,int temp_a){
for(i=0;i<8;i++){
temp_text[i]=text[act[1]+block_d][temp_a][temp_b+i];
}//for 
}//func 数値


void func_drawing_text_4(int temp_b,int temp_a){
for(i=0;i<8;i++){
temp_text[i]='\0';
}//for 
temp_text[0]=text[act[1]+block_d][temp_a][temp_b];
}//func 変数

void func_drawing_text_5(int temp_b,int temp_a){
switch(text[act[1]+block_d][temp_a][temp_b]){
case '1':strcpy(temp_text,"yes");break;
default :strcpy(temp_text,"no");break;
}//switch 
}//func 演算子




void draw_text_editer(int temp_a){//ここのtemp_aは現在選択しているブロックの番号
glColor3f(0.7,1.0,0.7);
glBegin(GL_POLYGON);
glVertex2f(0.8,0.3);
glVertex2f(-0.8,0.3);
glVertex2f(-0.8,-0.3);
glVertex2f(0.8,-0.3);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(0.8,0.3);
glVertex2f(-0.8,0.3);
glVertex2f(-0.8,-0.3);
glVertex2f(0.8,-0.3);
glEnd();

switch(block_type(temp_a)){
case 0:
func_drawing_text_4(0,0);draw_text_box(-0.6,0.2,0,0);
func_drawing_text_3(1,0);draw_text_strbox(-0.3,0.2,1,0);
func_drawing_text_1(9,0);draw_text_box(-0.2,0.2,9,0);
func_drawing_text_4(10,0);draw_text_box(-0.1,0.2,10,0);
func_drawing_text_3(11,0);draw_text_strbox(0.2,0.2,11,0);
func_drawing_text_2(19,0);draw_text_box(0.3,0.2,19,0);
func_drawing_text_4(20,0);draw_text_box(0.4,0.2,20,0);
func_drawing_text_3(21,0);draw_text_strbox(0.7,0.2,21,0);
break;

case 1:
func_drawing_text_4(0,0);draw_text_box(-0.6,0.2,0,0);
func_drawing_text_3(1,0);draw_text_strbox(-0.3,0.2,1,0);
func_drawing_text_4(9,0);draw_text_box(-0.1,0.2,9,0);
func_drawing_text_3(10,0);draw_text_strbox(0.2,0.2,10,0);
func_drawing_text_2(18,0);draw_text_box(0.3,0.2,18,0);
func_drawing_text_4(19,0);draw_text_box(0.4,0.2,19,0);
func_drawing_text_3(20,0);draw_text_strbox(0.7,0.2,20,0);
break;

case 2:
func_drawing_text_4(0,0);draw_text_box(-0.6,0.2,0,0);
func_drawing_text_3(1,0);draw_text_strbox(-0.3,0.2,1,0);
func_drawing_text_4(9,0);draw_text_box(-0.1,0.2,9,0);
func_drawing_text_3(10,0);draw_text_strbox(0.2,0.2,10,0);
func_drawing_text_2(18,0);draw_text_box(0.3,0.2,18,0);
func_drawing_text_4(19,0);draw_text_box(0.4,0.2,19,0);
func_drawing_text_3(20,0);draw_text_strbox(0.7,0.2,20,0);

func_drawing_text_4(0,1);draw_text_box(-0.6,0.0,0,1);
func_drawing_text_3(1,1);draw_text_strbox(-0.3,0.0,1,1);
func_drawing_text_1(9,1);draw_text_box(-0.2,0.0,9,1);
func_drawing_text_4(10,1);draw_text_box(-0.1,0.0,10,1);
func_drawing_text_3(11,1);draw_text_strbox(0.2,0.0,11,1);
func_drawing_text_2(19,1);draw_text_box(0.3,0.0,19,1);
func_drawing_text_4(20,1);draw_text_box(0.4,0.0,20,1);
func_drawing_text_3(21,1);draw_text_strbox(0.7,0.0,21,1);

func_drawing_text_4(0,2);draw_text_box(-0.6,-0.2,0,2);
func_drawing_text_3(1,2);draw_text_strbox(-0.3,-0.2,1,2);
func_drawing_text_4(9,2);draw_text_box(-0.1,-0.2,9,2);
func_drawing_text_3(10,2);draw_text_strbox(0.2,-0.2,10,2);
func_drawing_text_2(18,2);draw_text_box(0.3,-0.2,18,2);
func_drawing_text_4(19,2);draw_text_box(0.4,-0.2,19,2);
func_drawing_text_3(20,2);draw_text_strbox(0.7,-0.2,20,2);
break;

case 3:
func_drawing_text_5(0,0);draw_text_box(-0.6,0.2,0,0);
func_drawing_text_4(1,0);draw_text_box(-0.5,0.2,1,0);
func_drawing_text_3(2,0);draw_text_strbox(-0.2,0.2,2,0);
break;

case 4:
func_drawing_text_4(0,0);
draw_text_box(-0.6,0.2,0,0);
func_drawing_text_3(1,0);
draw_text_strbox(-0.3,0.2,1,0);
break;
case 5:
func_drawing_text_3(0,0);
draw_text_strbox(-0.4,0.2,0,0);
break;
}//switch
printf("%d",typing_p);printf("%s\n",text[act[1]+block_d][0]);
printf("%s\n",text[act[1]+block_d][1]);
printf("%s\n",text[act[1]+block_d][2]);

}//func

void select_a(){
for(select_addr[1]=select_addr[0];select_addr[1]<999;select_addr[1]++){//block_pの等しい　end句の位置を探す←中のやつと反応させないため
if(block[select_addr[1]]==10100&&block_p[select_addr[0]]==block_p[select_addr[1]]){break;}
}//for
if(select_addr[1]>998){select_addr[1]=998;}
}//func
void select(int temp_a){//ブロックの範囲選択　temp_aには現在選択しているブロックのアドレスを
select_addr[0]=temp_a;
switch(block[select_addr[0]]){//切り取るブロックの情報からend句まで切り取る必要があるかどうか
case 11900:select_a();break;
case 10200:select_a();break;
case 10501:select_a();break;
case 11500:select_a();break;
default:select_addr[1]=select_addr[0];break;//ない
}//switch
}//func


//select_addr[1]-select_addr[0]+1=選択された数
//  




void paste_blocks(int temp_a){
if(block[0]==0){temp_a=-1;}else{
while(1){
if(block[temp_a]==0){temp_a--;}else{break;}//貼り付けの時に詰める。
}//while
}//if

if(cpy_block[0]==862){printf("You can't put blocks on this place\n");return;}//ある特定のブロックは特定の場所にしか挿入できないため。
int temp_b;
for(temp_b=0;cpy_block[temp_b]!=0;temp_b++){}//temp_bずらすブロックの数、　コピーに入ってるブロックの数
for(i=999;i>temp_a;i--){//上にずらす
if(i+temp_b<999){
block[i+temp_b]=block[i];
for(j=0;j<3;j++){
strcpy(text[i+temp_b][j],text[i][j]);
}//for
}//if
}//for
for(i=0;cpy_block[i]!=0;i++){
block[temp_a+i+1]=cpy_block[i];//実際に貼り付ける
for(j=0;j<3;j++){
strcpy(text[temp_a+i+1][j],text[i][j]);
}//for
}//for
}//func

void remove_blocks(int temp_a){
select(temp_a);
if(block[select_addr[0]]==10000||block[select_addr[0]]==10100||block[select_addr[0]]==10502){
printf("Sorry you can't remove this block\n");return;}//一部の撤去できないブロックの選別
for(i=0;i<999;i++){
if(select_addr[1]+i>=999){//バッファオーバーフローしないために
block[select_addr[0]+i]=0;
for(j=0;j<3;j++){
strcpy(text[select_addr[0]+i][j],"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
}//for
}else{
block[select_addr[0]+i]=block[select_addr[1]+1+i];
for(j=0;j<3;j++){
strcpy(text[select_addr[0]+i][j],text[select_addr[1]+1+i][j]);
}//for
}//if

}//for
}//func

void copy_blocks(int temp_a){
for(i=0;i<999;i++){
cpy_block[i]=0;
for(j=0;j<3;j++){
strcpy(cpy_text[i][j],"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");//\0*29
}//for
}//for コピーの中身の初期化
select(temp_a);
for(i=select_addr[0];i<=select_addr[1];i++){//コピーする
cpy_block[i-select_addr[0]]=block[i];
for(j=0;j<3;j++){
strcpy(cpy_text[i-select_addr[0]][j],text[i][j]);
}//for
}//for
}//func

void draw_abc_box(float temp_a,float temp_b,int temp_c){//temp_aおよびbは座標　cは何番目のボックスだかをしめす、cと現在の選択値が一致するなら色を変える
glColor3f(1.0,1.0,1.0);
if(act[2]==temp_c){glColor3f(1.0,0.8,1.0);}//選択されている場合枠は紫
glBegin(GL_POLYGON);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.0375,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(1.0,1.0,0.0);
for(i=0;i<8;i++){
temp_text[i]=0;
}
temp_text[0]=65+current_abc;

RenderString(temp_a-0.009375,temp_b-0.009375,GLUT_BITMAP_TIMES_ROMAN_24,temp_text);
}//func

void draw_abc_strbox(float temp_a,float temp_b,int temp_c){
glColor3f(1.0,1.0,1.0);
if(act[2]==temp_c){glColor3f(1.0,0.8,1.0);}//選択されている場合枠は紫
glBegin(GL_POLYGON);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(temp_a+0.0375,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b+0.0375);
glVertex2f(temp_a-0.2625,temp_b-0.0375);
glVertex2f(temp_a+0.0375,temp_b-0.0375);
glEnd();
glColor3f(1.0,1.0,0.0);
for(i=0;i<8;i++){
temp_text[i]=0;
}//func

if(temp_c==1){//変数の型
switch(abc[current_abc]){
case 1:strcpy(temp_text,"int");break;
case 2:strcpy(temp_text,"float");break;
case 3:strcpy(temp_text,"char");break;
default:strcpy(temp_text,"none");break;
}//switch
}else{//変数の配列数指定
for(i=0;i<8;i++){
temp_text[i]=abc2[current_abc][i];
}//for 
}//if

RenderString(temp_a-0.209375,temp_b-0.009375,GLUT_BITMAP_TIMES_ROMAN_24,temp_text);
}

void draw_abc_setting(){
glColor3f(0.7,1.0,0.7);
glBegin(GL_POLYGON);
glVertex2f(0.8,0.3);
glVertex2f(-0.8,0.3);
glVertex2f(-0.8,-0.3);
glVertex2f(0.8,-0.3);
glEnd();
glColor3f(0.0,1.0,0.0);
glBegin(GL_LINE_LOOP);
glVertex2f(0.8,0.3);
glVertex2f(-0.8,0.3);
glVertex2f(-0.8,-0.3);
glVertex2f(0.8,-0.3);
glEnd();
draw_abc_box(-0.6,0.0,0);
draw_abc_strbox(-0.3,0.0,1);
draw_abc_strbox(0.0,0.0,2);

}

void display(){
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1.0,1.0,0.8);
glBegin(GL_POLYGON);
glVertex2f(-1,-1);
glVertex2f(1,-1);
glVertex2f(1,1);
glVertex2f(-1,1);
glEnd();
glColor3f(0.9,1.0,1.0);
glBegin(GL_POLYGON);//編集画面
glVertex2f(-0.5,-0.4);
glVertex2f(0.9,-0.4);
glVertex2f(0.9,0.4);
glVertex2f(-0.5,0.4);
glEnd();

draw_blocks();

glColor3f(1.0,1.0,0.8);//マスク
glBegin(GL_POLYGON);
glVertex2f(-1,-1);
glVertex2f(-0.5,-1);
glVertex2f(-0.5,1);
glVertex2f(-1,1);
glEnd();
draw_panels();
draw_panels_b();
draw_moving_panels();
draw_current_panel();
if(page==2){
draw_text_editer(act[1]+block_d);
}
if(page==1){
draw_abc_setting();
}
glFlush();
}

void resize(int resize_w,int resize_h){
windowsize_w=1.0*resize_w;
windowsize_h=1.0*resize_h;
glViewport(0,0,resize_w,resize_h);//描画領域指定　4つの引数より、リサイズ後の画面全体を使うことに
glLoadIdentity();
glOrtho(-resize_w/1280.0 , resize_w/1280.0 , -resize_h/1280.0 , resize_h/1280.0 , -1.0,1.0);
}

void keyboard_n(unsigned char key,int mouse_x_0,int mouse_y_0){
if(key=='\033'){
act[2]=0;
act[3]=0;
page=0;
}
if(page==0){
if(key==13||key==10){
act[2]=0;
page=1;
}//変数設定を開く
if(act[0]==4){printf("CTRL");//glutGetModifiers()&GLUT_ACTIVE_CTRL&&
switch(key){
case 'x':printf("cut");copy_blocks(act[1]+block_d);remove_blocks(act[1]+block_d);block_positioning();display();break;//切り取り
case 'c':printf("copy");copy_blocks(act[1]+block_d);display();break;//コピー
case 'v':printf("paste");paste_blocks(act[1]+block_d);block_positioning();display();break;//貼り付け
default:printf("???");break;
}//switch
}//if
if(key==' '){
printf("space");//デバッグ用
if(act[0]==0){
switch(act[1]){
case 0:break;//save_file();
case 1:delete_blocks();block_positioning();break;//open_file();
case 2:translating();break;
case 3:delete_blocks();break;//配列の初期化
default:break;
}//switch
}
if(act[0]==1){
switch(act[1]){
case 0:if(act[4]>0){act[4]--;}break;
case 1:act[4]++;break;
}//switch
}
if(act[0]==2){
switch(act[1]){
case 0:block_d=block_d-8;if(block_d<0){block_d=0;}break;
case 1:block_d=block_d+8;if(block_d>982){block_d=982;}break;
}//switch
}
if(act[0]==3){
for(i=0;i<999;i++){
cpy_block[i]=0;
for(j=0;j<3;j++){
for(k=0;k<29;k++){
text[i][j][k]=0;
}//for k
}//for
}//for コピーの中身の初期化

switch(act[1]){
case 0:cpy_block[0]=10501;cpy_block[1]=10502;cpy_block[2]=10100;break;
case 1:cpy_block[0]=10503;break;
case 2:cpy_block[0]=10200;cpy_block[1]=10100;break;
case 3:cpy_block[0]=11900;cpy_block[1]=10100;break;
case 4:cpy_block[0]=11500;cpy_block[1]=10000;cpy_block[2]=10100;break;
case 5:cpy_block[0]=9900;break;
case 6:cpy_block[0]=9800;break;
case 7:cpy_block[0]=11200;break;
case 8:cpy_block[0]=11501;break;
case 9:cpy_block[0]=10900;break;
}//switch
}//if
if(act[0]==4){
page=2;printf("text edit");
}//if

}//if

}//if page==0

if(page==1){
if(act[2]==0){
if((key>='A'&&key<='Z')||(key<='z'&&key>='a')){
current_abc=key-65;
}
}//act[2]==0
if(act[2]==1){
if(key==' '){
if(abc[current_abc]<0||abc[current_abc]>3){abc[current_abc]=0;}
abc[current_abc]++;
if(abc[current_abc]<0||abc[current_abc]>3){abc[current_abc]=0;}
}//if
}//act[2]==1
if(act[2]==2){

if(key==8){
if(typing_p>0){
typing_p--;
abc2[current_abc][typing_p]='\0';}
}else{
if(typing_p<8&&key>='0'&&key<='9'){
abc2[current_abc][typing_p]=key;
typing_p++;}
}
if(typing_p>8){
typing_p=8;}

}//act[2]==2
}//page=1



if(page==2){//記述の中身
switch(text_type()){
case 1://変数
if((key>='A'&&key<='Z')||(key<='z'&&key>='a')){
text[act[1]+block_d][act[3]][act[2]]=key;
}else{

if(key==8){
text[act[1]+block_d][act[3]][act[2]]='\0';
}//if

}//if
break;//case 1

case 2:
if(key==' '){
text[act[1]+block_d][act[3]][act[2]]++;
if(text[act[1]+block_d][act[3]][act[2]]<'0'||text[act[1]+block_d][act[3]][act[2]]>'4'){text[act[1]+block_d][act[3]][act[2]]='0';}
}//if
break;//case 2 
case 3:
if(key==' '){
text[act[1]+block_d][act[3]][act[2]]++;
if(text[act[1]+block_d][act[3]][act[2]]<'0'||text[act[1]+block_d][act[3]][act[2]]>'5'){text[act[1]+block_d][act[3]][act[2]]='0';}
}//if
break;//case 3

case 4:
if(key==8){
if(typing_p>0){
typing_p--;
text[act[1]+block_d][act[3]][typing_p+act[2]]='\0';}
}else{
if(typing_p<8){
text[act[1]+block_d][act[3]][typing_p+act[2]]=key;
typing_p++;}
}
if(typing_p>8){
typing_p=8;}
break;//case 4

case 6:
if(key==' '){
text[act[1]+block_d][act[3]][act[2]]++;
if(text[act[1]+block_d][act[3]][act[2]]<'0'||text[act[1]+block_d][act[3]][act[2]]>'1'){text[act[1]+block_d][act[3]][act[2]]='0';}
}//if
break;//case 6
}//switch
}//if page=2
display();
}//func







void push_key_horizontal(int temp_a){
if(page==0){
act[1]=0;
act[0]=act[0]+temp_a;
if(act[0]==5){act[0]=0;}
if(act[0]==-1){act[0]=4;}
}//if page=0

if(page==1){
act[2]=act[2]+temp_a;
if(act[2]==3){act[2]=0;}
if(act[2]==-1){act[2]=2;}
typing_p=strlen(abc2[current_abc]);
}

if(page==2){
switch(block_type(act[1]+block_d)){
case 0:
act[2]=act[2]+temp_a;
if(act[2]==2){act[2]=9;}
if(act[2]==8){act[2]=1;}
if(act[2]==12){act[2]=19;}
if(act[2]==18){act[2]=11;}
if(act[2]==22){act[2]=0;}
if(act[2]==-1){act[2]=21;}
break;
case 1:
act[2]=act[2]+temp_a;
if(act[2]==2){act[2]=9;}
if(act[2]==8){act[2]=1;}
if(act[2]==11){act[2]=18;}
if(act[2]==17){act[2]=10;}
if(act[2]==21){act[2]=0;}
if(act[2]==-1){act[2]=20;}
break;
case 2:
act[2]=act[2]+temp_a;
if(act[3]==1){//forの条件式の場合
if(act[2]==2){act[2]=9;}
if(act[2]==8){act[2]=1;}
if(act[2]==12){act[2]=19;}
if(act[2]==18){act[2]=11;}
if(act[2]==22){act[2]=0;}
if(act[2]==-1){act[2]=21;}
}else{
if(act[2]==2){act[2]=9;}
if(act[2]==8){act[2]=1;}
if(act[2]==11){act[2]=18;}
if(act[2]==17){act[2]=10;}
if(act[2]==21){act[2]=0;}
if(act[2]==-1){act[2]=20;}
}
break;
case 3:
act[2]=act[2]+temp_a;
if(act[2]==-1){act[2]=2;}
if(act[2]==3){act[2]=0;}
break;
case 4:
act[2]=act[2]+temp_a;
if(act[2]==-1){act[2]=1;}
if(act[2]==2){act[2]=0;}
break;
case 5:
act[2]=0;
break;
}//switch
}//if page=2
}//func

void push_key_vertical(int temp_a){
if(page==0){
printf("temp_a %d\n",temp_a);
switch(act[0]){
case 0:printf("case 0\n");act[1]=act[1]+temp_a;if(act[1]==4){act[1]=0;}if(act[1]==-1){act[1]=3;}break;
case 1:act[1]=act[1]+temp_a;if(act[1]==2){act[1]=0;}if(act[1]==-1){act[1]=1;}break;
case 2:act[1]=act[1]+temp_a;if(act[1]==2){act[1]=0;}if(act[1]==-1){act[1]=1;}break;
case 3:act[1]=act[1]+temp_a;if(act[1]==10){act[1]=0;}if(act[1]==-1){act[1]=9;}break;
case 4:act[1]=act[1]+temp_a;if(act[1]==16){act[1]=15;if(block_d!=982){block_d++;}
}if(act[1]==-1){act[1]=0;if(block_d!=0){block_d--;}
}break;
default: printf("default\n");break;
}//switch
}//if page=0


if(page==2){
if(block_type(act[1]+block_d)==2){
act[2]=0;
act[3]=act[3]+temp_a;if(act[3]==3){act[3]=0;}if(act[3]==-1){act[3]=2;}
}//if
}//if page=2

}//func

void keyboard_s(int key,int mouse_x_0,int mouse_y_0){
int temp_a;//正と負,上か下か右か左かどちらを区別するための一時的なもの
switch(key){
case GLUT_KEY_RIGHT:temp_a=1;push_key_horizontal(temp_a);printf("right %d %d\n",act[0],act[1]);break;
case GLUT_KEY_LEFT:temp_a=-1;push_key_horizontal(temp_a);printf("left %d %d\n",act[0],act[1]);break;
case GLUT_KEY_UP:temp_a=-1;push_key_vertical(temp_a);printf("up %d %d\n",act[0],act[1]);break;
case GLUT_KEY_DOWN:temp_a=1;push_key_vertical(temp_a);printf("down %d %d\n",act[0],act[1]);break;
default:break;
}//switch
if(page==2){
typing_p=0;
for(i=0;text[act[1]+block_d][act[3]][i+act[2]]!='\0';i++){//strlen()のようなもの
typing_p=i;
}//page=2
}
display();
}

int main(int argc, char *argv[]){    // 主関数(プログラムのスタート)
  for(i=0;i<58;i++){  
  for(j=0;j<8;j++){
  abc2[i][j]=0;
  }}
  delete_blocks();
  //open_file();
  //block_positioning();
  act[0]=3;act[1]=0;act[2]=0;act[3]=0;
  glutInit(&argc, argv);            // グラフィック初期化
  glutInitDisplayMode(GLUT_RGBA);   // 画素毎に色を指定する描画モード
  glutInitWindowSize( 1280,640 );    // ウィンドウの大きさ指定 10:5
  glutCreateWindow(window_name);        // 描画ウィンドウ作成(引数は表示される文字列を与える)
  glutDisplayFunc(display);         // 描画関数指定
  glutReshapeFunc(resize);          // サイズ変更時におこなう関数指定、引数にはリサイズ後の高さと横幅を渡す。
  glutKeyboardFunc(keyboard_n);
  glutSpecialFunc(keyboard_s);
  glClearColor(1.0, 1.0, 1.0, 1.0); // 背景色指定(白)
  glutMainLoop();
  return 0;
}







