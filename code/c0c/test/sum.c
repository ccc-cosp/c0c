int sum(int n) {
  int s, i;
  s = 0;
  i = 1;
  while (i <= n) {
    s = s + i;
    i = i + 1;
  }
  return s;
}
