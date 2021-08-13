#include<stdarg.h>
double add( int n, ... )
{
  int i = 0;
  double sum = 0.0;
  va_list argptr;
  va_start( argptr, n );             // 初始化argptr
  for ( i = 0; i < n; ++i )       // 对每个可选参数，读取类型为double的参数，
    sum += va_arg( argptr, double ); // 然后累加到sum中
  va_end( argptr );
  return sum;
}
int main(){
	return 0;
}
