#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>

#include "sculptor.h"
#include "voxel.h"

// construtor
Sculptor::Sculptor(int _nx, int _ny, int _nz) {
  // cadastrando posições
  nx = _nx;
  ny = _ny;
  nz = _nz;

  // alocando matriz v
  v = new Voxel **[nx];

  for (int i = 0; i < nx; i++) {
    v[i] = new Voxel *[ny];
  }

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      v[i][j] = new Voxel[nz];
    }
  }

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        v[i][j][k].isOn = false; // desativando os voxels
      }
    }
  }
}

// destrutor
Sculptor::~Sculptor() {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      delete[] v[i][j]; // deletando memoria
    }
  }

  for (int i = 0; i < nx; i++) {
    delete[] v[i]; // deletando memoria
  }

  delete[] v; // deletando memoria

  // Zerando posições
  nx = 0;
  ny = 0;
  nz = 0;
}

// seleciona a cor
void Sculptor::setColor(float r, float g, float b, float a) {
  // cadastrando cores(RGB) e tranparencia
  this->r = r;  this->g = g;  this->b = b;  this->a = a;
}

// progeta o voxel
void Sculptor::putVoxel(int x, int y, int z) {
  // ativa voxel
  v[x][y][z].isOn = true;

  // registra cor(RGB) e tranparencia do voxel
  v[x][y][z].r = r;
  v[x][y][z].g = g;
  v[x][y][z].b = b;
  v[x][y][z].a = a;
}

// deleta voxel
void Sculptor::cutVoxel(int x, int y, int z) {
  // desativa voxel
  v[x][y][z].isOn = false;
}

// projeta caixa de voxels
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1) {
  for (int i = x0; i < x1; i++) {
    for (int j = y0; j < y1; j++) {
      for (int k = z0; k < z1; k++) {
        putVoxel(i, j, k); // adiciona voxels da caixa
      }
    }
  }
}

// deleta caixa
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1) {
  for (int i = x0; i < x1; i++) {
    for (int j = y0; j < y1; j++) {
      for (int k = z0; k < z1; k++) {
        cutVoxel(i, j, k); // desativa todos os voxels
      }
    }
  }
}

// projeta esfera
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius) {

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        // calcula equação da esfera
        float eq =
            (pow(i - xcenter, 2) + pow(j - ycenter, 2) + pow(k - zcenter, 2));
        float r2 = pow(radius, 2); // raio ao quadrado

        // se resultado da equação for menor ou igual ao raio ao quadrado
        if (eq <= r2) {
          putVoxel(i, j, k); // add voxels
        }
      }
    }
  }
}

// deleta esfera
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius) {

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        // calcula equação da esfera
        float eq =
            (pow(i - xcenter, 2) + pow(j - ycenter, 2) + pow(k - zcenter, 2));
        float r2 = pow(radius, 2); // raio ao quadrado

        // se resultado da equação for menor ou igual ao raio ao quadrado
        if (eq <= r2) {
          cutVoxel(i, j, k); // deleta voxels
        }
      }
    }
  }
}

// projeta elipse
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx,int ry, int rz) {

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nx; k++) {
        // calcula equação da esfera
        float eq =
            ((pow((float)(xcenter-i)/(rx), 2) ) + 
            (pow((float)(ycenter-j)/(ry), 2)) +
            (pow((float)(zcenter-k)/(rz), 2) ));

        // se resultado da equação for menor ou igual ao raio ao quadrado
        if (eq <= 1) {
          putVoxel(i, j, k); // desenha voxels
        }
      }
    }
  }
}
// deleta elipse
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx,int ry, int rz) {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        // calcula equação da esfera
        float eq =
            ((pow((float)(xcenter-i)/(rx), 2) ) + 
            (pow((float)(ycenter-j)/(ry), 2)) +
            (pow((float)(zcenter-k)/(rz), 2) ));

        // se resultado da equação for menor ou igual ao raio ao quadrado
        if (eq <= 1) {
          cutVoxel(i, j, k); // deleta voxels
        }
      }
    }
  }
}
// cria e escreve no arquivo
void Sculptor::writeOFF(const char *filename) {

  // criando fluxo de arquivo
  std::ofstream fout;

  int vt = 0;
  int f = 0;
  int tmp = 0;

  // abrindo arquivo
  fout.open(filename);

  fout << "OFF" << std::endl; // escreve o tipo do arquivo

  // definindo dimensões
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        if (v[i][j][k].isOn == true) {
          vt = vt + 8; // oito vertices
          f = f + 6;   // seis faces
        }
      }
    }
  }

  // escrevendo as dimensões
  // NVertices       NFaces          NArestas
  fout << vt << " " << f << " " << 0 << std::endl;

  // fixando as cores em 2 casas decimais para forçar aparecer em ponto
  // flutuante
  fout << std::fixed;
  fout << std::setprecision(2);

  // loop que percorre a matriz 3D
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        // se o voxel estiver ativo
        if (v[i][j][k].isOn == true) {

          // Escreve as coordenadas espaciais tridimensionais dos NVertices
          fout << i - 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
          fout << i - 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
          fout << i + 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
          fout << i + 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
          fout << i - 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
          fout << i - 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
          fout << i + 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
          fout << i + 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
        }
      }
    }
  }

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        if (v[i][j][k].isOn == true) {
          // escreve especificações das faces
          //      Nv          v[0]            v[1]                    ...
          //      v[Nv-1]            r g b a
          fout << 4 << " " << tmp + 0 << " " << tmp + 3 << " " << tmp + 2 << " "
               << tmp + 1 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          fout << 4 << " " << tmp + 4 << " " << tmp + 5 << " " << tmp + 6 << " "
               << tmp + 7 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          fout << 4 << " " << tmp + 0 << " " << tmp + 1 << " " << tmp + 5 << " "
               << tmp + 4 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          fout << 4 << " " << tmp + 0 << " " << tmp + 4 << " " << tmp + 7 << " "
               << tmp + 3 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          fout << 4 << " " << tmp + 3 << " " << tmp + 7 << " " << tmp + 6 << " "
               << tmp + 2 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          fout << 4 << " " << tmp + 1 << " " << tmp + 2 << " " << tmp + 6 << " "
               << tmp + 5 << " " << v[i][j][k].r << " " << v[i][j][k].g << " "
               << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
          tmp += 8;
        }
      }
    }
  }

  // fecha arquivo
  fout.close();

  std::cout << ".off salvo com sucesso" << std::endl;
}