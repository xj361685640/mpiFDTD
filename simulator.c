#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"
#include "field.h"
#include "mpiTM_UPML.h"
#include "mpiTE_UPML.h"
#include "drawer.h"
#include "models.h"
#include <sys/time.h>

static void (*update)() = NULL;
static double complex* (*getDataX)() = NULL;
static double complex* (*getDataY)() = NULL;
static double complex* (*getDataZ)() = NULL;
static void (* finishMethod)() = NULL;
static void (* initMethod)() = NULL;
static void (* getSubFieldPositionMethod)(int*,int*,int*,int*) = NULL;
static double complex* (*getDrawData)() = NULL;
static double* (* getEpsMethod )() = NULL;

static struct timeval timer1, timer2;

static void setTM()
{
}

static void setTE()
{
}

static void setTMupml()
{
}

static void setTEupml()
{
}

static void setMPITMupml(){
  update       = mpi_fdtdTM_upml_getUpdate();
  initMethod   = mpi_fdtdTM_upml_getInit();
  finishMethod = mpi_fdtdTM_upml_getFinish();
  
  getSubFieldPositionMethod = mpi_fdtdTM_upml_getSubFieldPositions;
  getEpsMethod              = mpi_fdtdTM_upml_getEps;
  
  getDataX = mpi_fdtdTM_upml_getHx;
  getDataY = mpi_fdtdTM_upml_getHy;
  getDataZ = mpi_fdtdTM_upml_getEz;
  
  getDrawData = getDataZ;
}

static void setMPITEupml(){
  update       = mpi_fdtdTE_upml_getUpdate();
  initMethod   = mpi_fdtdTE_upml_getInit();
  finishMethod = mpi_fdtdTE_upml_getFinish();
  
  getSubFieldPositionMethod = mpi_fdtdTE_upml_getSubFieldPositions;
  getEpsMethod              = mpi_fdtdTE_upml_getEps;

  getDataX = mpi_fdtdTE_upml_getEx;
  getDataY = mpi_fdtdTE_upml_getEy;
  getDataZ = mpi_fdtdTE_upml_getHz;

  getDrawData = getDataY;
}

static void setSolver(enum SOLVER solver)
{
  switch(solver){
  case TM_2D:
    break;
  case TE_2D:
    break;
  case MPI_TM_UPML_2D:
    setMPITMupml();
    break;
  case MPI_TE_UPML_2D:
    setMPITEupml();
    break;
  default:
    printf("error, not implement simulator (simulator.c)\n");
    exit(2);
    break;
  }
  (*initMethod)(); //Solverの初期化, EPS, Coeffの設定  
}

void simulator_calc(){
  (*update)();
  
  field_nextStep();   //時間を一つ進める  
}

void simulator_init(FieldInfo field_info, enum MODEL model, enum SOLVER solver)
{
  //横幅(nm), 縦幅(nm), 1セルのサイズ(nm), pmlレイヤの数, 波長(nm), 計算ステップ
  initField(field_info);

  //NO_MODEL. MIE_CYLINDER, SHELF, NONSHELF
  // UNDONE : SHELF, NONSHELFモデル
  setModel(model);  //次にこれ,  モデル(散乱体)を定義

  setSolver(solver);//Solverの設定と初期化

  gettimeofday(&timer1, NULL); //開始時間の取得
}

void simulator_finish()
{  
  printf("finish at %d step \n", (int)field_getTime());
  gettimeofday(&timer2,NULL);
  printf("time = %lf \n", timer2.tv_sec-timer1.tv_sec+(timer2.tv_usec-timer1.tv_usec)*1e-6);

  (*finishMethod)();   //メモリの解放等, solverの終了処理
}

double complex* simulator_getDrawingData(void){
  return (* getDrawData)();
}

bool simulator_isFinish(void)
{
  return field_isFinish();
}

//小領域の幅と高さを取ってくる
void simulator_getSubFieldPositions(int *subNx,int *subNy,int *subNpx, int *subNpy)
{
  (*getSubFieldPositionMethod)(subNx, subNy, subNpx, subNpy);  
}

//屈折率のマップを取ってくる
double *simulator_getEps()
{
  return getEpsMethod();  
}
