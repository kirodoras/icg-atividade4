//------------------------
// Universidade Federal de Sergipe
// Departamento de Matemática - Prof Evilson Vieira
// Transformação linear
// compilar: gcc -o fator fator.c -lm
// executar: ./fator gatopreto.ppm
// Aluno Mateus Figueiredo Pereira
// Aracaju-SE 26/12/2023
//------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(int argc, char **argv)
{
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  FILE *fp;

  fp = fopen(argv[1], "r");
  while ((caractere = getc(fp)) == '#')
    while ((caractere = getc(fp)) != '\n')
      ;
  ungetc(caractere, fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((caractere = getc(fp)) == '#')
    while ((caractere = getc(fp)) != '\n')
      ;
  ungetc(caractere, fp);

  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);

  unsigned char(**imagem)[3];

  j = l * sizeof(char *);
  imagem = malloc(j);

  j = h * 3;
  for (i = 0; i < l; i++)
    imagem[i] = malloc(j);

  if (type == 3)
  {
    for (j = 0; j < h; j++)
      for (i = 0; i < l; i++)
        fscanf(fp, "%hhu %hhu %hhu", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
    fclose(fp);
  }
  else if (type == 6)
  {
    for (j = 0; j < h; j++)
      for (i = 0; i < l; i++)
        fscanf(fp, "%c%c%c", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
    fclose(fp);
  }
  else
  {
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }

  int L = 0, H = 0;
  float fator =  1.7;
  unsigned char(**transformada)[3];
  L = (int)(fator * l);
  H = (int)(fator * h);
  j = L * sizeof(char *);
  transformada = malloc(j);

  j = H * 3;
  for (i = 0; i < L; i++)
    transformada[i] = malloc(j);

  float matriz_inversa[2][2] = {
      {1/fator, 0},
      {0, 1/fator}};

  int io = 0, jo = 0, ic = 0, jc = 0;

  ic = l >> 1;
  jc = h >> 1;

  for (j = 0; j < H; j++)
  {
    for (i = 0; i < L; i++)
    {
      io = matriz_inversa[0][0] * (i - L / 2) + ic;
      jo = matriz_inversa[1][1] * (j - H / 2) + jc;

      if (io >= 0 && io < l && jo >= 0 && jo < h)
      {
        transformada[i][j][0] = imagem[io][jo][0];
        transformada[i][j][1] = imagem[io][jo][1];
        transformada[i][j][2] = imagem[io][jo][2];
      }
      else
      {
        transformada[i][j][0] = 0;
        transformada[i][j][1] = 0;
        transformada[i][j][2] = 0;
      }
    }
  }

  fp = fopen("output.ppm", "w");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n255\n", L, H);
  for (j = 0; j < H; j++)
    for (i = 0; i < L; i++)
      fprintf(fp, "%c%c%c", transformada[i][j][0], transformada[i][j][1], transformada[i][j][2]);
  fclose(fp);

  for (i = 0; i < l; i++)
    free(imagem[i]);
  free(imagem);

  for (i = 0; i < L; i++)
    free(transformada[i]);
  free(transformada);
}