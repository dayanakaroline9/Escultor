#include <fstream>
#include <iostream>

#include "sculptor.h"
#include "voxel.h"

// teste da classe Sculptor, construindo um foguete

int main() {
  // cria um escultor cuja matriz tem 50x50x50 voxels
  Sculptor t(100, 100, 100);

  // seleciona cor
  t.setColor(0, 0, 1.0, 1.0); // azul
  // constroi elipse com centro em (50,50,50) com Rx=25, Ry=50 e Ry=25;
  t.putEllipsoid(50, 50, 50, 25, 50, 25);
  t.setColor(1.0, 0, 0, 1.0); // vermelho
  // constroi espera com centro em (65,65,65) com raio = 10
  t.putSphere(65, 65, 65, 10);
  // deleta voxel na area de uma caixa, que vai de 0 ate 100 para x e z e para y
  // apenas de 0 a 20
  t.cutBox(0, 100, 0, 20, 0, 100);

  // corta area interna da espera criada em (65,65,65) com raio 10
  t.cutSphere(65, 65, 65, 8);
  // cria caixa que vai de 65 ate 75 para x, de 0 a 40 para y e de 45 a 55 para
  // z
  t.putBox(65, 75, 0, 40, 45, 55);
  // cria caixa que vai de 45 a 55 para x, de 0 a 40 para y e de 65 ate 75  para
  // z
  t.putBox(45, 55, 0, 40, 65, 75);
  // cria caixa que vai de 45 a 55 para x, de 0 a 40 para y e de 25 a 35 para z
  t.putBox(45, 55, 0, 40, 25, 35);
  // cria caixa que vai de 25 a 35 para x, de 0 a 40 para y e de 45 a 55 para z
  t.putBox(25, 35, 0, 40, 45, 55);

  // deleta are intera na elipse, com raios menores rx=15, ry=40 e rz=15
  t.cutEllipsoid(50, 50, 50, 15, 40, 15);

  // grava a escultura digital no arquivo "teste.off"
  t.writeOFF("teste.off");

  return 0;
}