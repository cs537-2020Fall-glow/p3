#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  char a[3840+4096];
  char b[4096];
  char c[4096];
  char d[4096];
  char e[4096];
  // char f[4096];
  // char g[4096];
  // char h[4096];
  // char i[4096];
  // char j[4096];
  // char k[4096];
  // char l[4096];
  // char m[4096];
  // char n[4096];
  // char o[4096];
  // char p[4096];
  // char q[4096];
  // char r[4096];
  // char s[4096];
  // char t[4096];
  // char u[4096];
  // char v[4096];
  // char w[4096];
  // char x[4096];
  // char y[4096];
  // char z[4096];
  // b[0] = 0;
  // c[0] = 0;
  // d[0] = 0;
  // e[0] = 0;
  // f[0] = 0;
  // g[0] = 0;
  // h[0] = 0;
  // i[0] = 0;
  // j[0] = 0;
  // k[0] = 0;
  // l[0] = 0;
  // m[0] = 0;
  // n[0] = 0;
  // o[0] = 0;
  // p[0] = 0;
  // q[0] = 0;
  // r[0] = 0;
  // s[0] = 0;
  // t[0] = 0;
  // u[0] = 0;
  // v[0] = 0;
  // w[0] = 0;
  // x[0] = 0;
  // y[0] = 0;
  printf(1, "addr a[0]: %x\n", &a);
  printf(1, "addr b[0]: %x\n", &b);
  printf(1, "addr c[0]: %x\n", &c);
  printf(1, "addr d[4096]: %x\n", &d[4096]);
  printf(1, "addr e[4096]: %x\n", &e[4096]);

  exit();
}