/* Oráculos SEQUENCIAIS dos contratos de saída, não gabaritos concorrentes.
 * Servem para testar o corretor e os dados. O aluno deve escrever as primitivas.
 */
#define main legado_main
#include "contratos.c"
#undef main
static int eq(const char *s){return strcmp(EX,s)==0;}
static void lote(int c,int v,int n,int sai,int unicos){
 for(int r=0;r<v;r++){
  for(int j=0;j<c;j++)printf("ENTRA %d\n",unicos?r*c+j:(r*c+j)%n);
  puts("INICIO\nFIM");
  if(sai)for(int j=0;j<c;j++)printf("SAI %d\n",unicos?r*c+j:(r*c+j)%n);
 }
}
int main(void){
 int n=0,k=0,m=0,q=0,v=0,a=0,b=0;long long x=0;
 if(eq("s01")){scanf("%d",&k);for(int i=1;i<=5*k;i++)printf("%d\n",i);}
 else if(eq("s02"))puts("F1\nF2\nF8\nF5\nF9\nF6\nF3\nF7\nF4");
 else if(eq("s03"))puts("B\nA\nC\nE\nD");
 else if(eq("s11"))puts("F1\nF4\nF3\nF6\nF2\nF5\nF7");
 else if(eq("s04")){scanf("%d",&v);lote(20,v,20*v,0,1);}
 else if(eq("s05")){scanf("%d%d%d",&n,&m,&v);lote(m,v,n,1,0);}
 else if(eq("s07")){scanf("%d",&v);lote(10,v,10*v,1,1);}
 else if(eq("s06")){
  scanf("%d%d%d",&n,&m,&k);int estoque=m;
  for(int i=0;i<n;i++)for(int j=0;j<k;j++){if(!estoque){puts("REPOE");estoque=m;}printf("COME %d\n",i);estoque--;}
 }
 else if(eq("s08")){
  scanf("%d%d%d",&n,&m,&k);int valor=0;
  for(int j=0;j<k;j++){
   for(int i=0;i<n;i++)printf("RI %d %d\nRF %d\n",i,valor,i);
   for(int i=0;i<m;i++)printf("WI %d\nWF %d %d\n",i,i,++valor);
  }
 }
 else if(eq("s09")||eq("m04")){
  int fila[100],head=0,tail=0,atual=-1;char op;
  scanf("%d",&q);
  while(q--){scanf(" %c",&op);
   if(op=='A'){
    scanf("%d",&a);
    if(atual<0){atual=a;printf("ATENDE %d\n",a);}
    else if(tail-head<5){fila[tail++]=a;printf("ESPERA %d\n",a);}
    else printf("RECUSA %d\n",a);
   }else if(atual<0)puts("LIVRE");
   else{printf("FIM %d\n",atual);atual=head<tail?fila[head++]:-1;if(atual<0)puts("LIVRE");else printf("ATENDE %d\n",atual);}
  }
 }
 else if(eq("s10")||eq("m05")||eq("s15")){
  if(eq("s15"))scanf("%d%d%d",&n,&m,&k);else{n=5;scanf("%d",&k);}
  for(int j=0;j<k;j++)for(int i=0;i<n;i++)printf("IN %d\nOUT %d\n",i,i);
 }
 else if(eq("s12")){
  int disponivel[8],demanda[8];char op;
  scanf("%d%d",&m,&q);for(int i=0;i<m;i++)scanf("%d",&disponivel[i]);
  while(q--){int ok=1;scanf(" %c",&op);for(int i=0;i<m;i++){scanf("%d",&demanda[i]);if(demanda[i]>disponivel[i])ok=0;}
   if(op=='U'){for(int i=0;i<m;i++)disponivel[i]+=demanda[i];puts("OK");}
   else if(ok){for(int i=0;i<m;i++)disponivel[i]-=demanda[i];puts("OK");}else puts("FALHA");
  }
  printf("ESTADO");for(int i=0;i<m;i++)printf(" %d",disponivel[i]);puts("");
 }
 else if(eq("s13")){scanf("%d",&q);while(q--){scanf("%d%d",&a,&b);printf("FUMA %d\n",3-a-b);}}
 else if(eq("s14")){scanf("%d%d",&n,&k);while(k--){scanf("%lld",&x);printf("%lld\n",x);}}
 else if(eq("m01")||eq("m03")){scanf("%d%d%d%d",&n,&a,&b,&k);for(int i=a*k-1;i>=0;i--)printf("ITEM %d\n",i);}
 else if(eq("m02")){
  scanf("%d%d",&m,&n);
  for(int i=0;i<n;i++)printf("PE %d\nEN %d\nAT %d\nPS %d\nSA %d\nFI %d\n",i,i,i,i,i,i);
 }
 else if(eq("m06")){
  scanf("%d%d",&n,&k);
  for(int j=0;j<k;j++){for(int i=n-1;i>=0;i--)printf("CHEGA %d %d\n",j,i);for(int i=0;i<n;i++)printf("PASSA %d %d\n",j,i);}
 }
 else if(eq("p04")){scanf("%d",&n);printf("BYTES %d\n",n);}
 else if(eq("p05")||eq("p08")||eq("t01")){
  scanf("%d",&k);for(int i=1;i>=0;i--)for(int j=0;j<k;j++)printf("%d %d\n",i,j);
 }
 else if(eq("p06")){
  char linha[256];int total=0;scanf("%d",&n);fgets(linha,sizeof linha,stdin);
  for(int i=0;i<n;i++){if(!fgets(linha,sizeof linha,stdin))return 2;if(strstr(linha,"SO"))total++;}printf("%d\n",total);
 }
 else if(eq("p07")){scanf("%d",&n);printf("%d %lld\n",n,120LL*n);}
 else if(eq("t02")){scanf("%d",&k);for(int i=0;i<k;i++)printf("TRABALHO %d\n",i);puts("FIM");}
 else if(eq("t03")){for(int i=0;i<4;i++){scanf("%d%d",&a,&b);long long soma=0;for(int j=a;j<b;j++)soma+=j;printf("%d %lld\n",i,soma);}}
 else if(eq("t04")){for(int i=0;i<10;i++){scanf("%lld",&x);printf("%d %lld\n",i,x);}}
 else if(eq("t05")){for(int i=0;i<4;i++){scanf("%lld",&x);printf("%lld ",1+x%6);}puts("");}
 else if(eq("t08")){
  scanf("%d%d",&m,&n);
  if(!m){for(int i=0;i<n;i++)printf("INICIO %d\nFIM %d\n",i,i);}
  else{for(int i=0;i<n;i++)printf("CHEGA 0 %d\n",i);for(int i=n-1;i>=0;i--)printf("PASSA 0 %d\n",i);}
 }
 else if(eq("t11")){scanf("%d",&n);for(int i=0;i<n;i++){scanf("%lld",&x);printf("%lld ",2*x);}puts("");}
 else if(eq("x01")){scanf("%d",&n);for(int i=0;i<n;i++){scanf("%lld",&x);printf("%d %lld\n",i,x);}}
 else if(eq("x05"))puts("OCUPADO\nADQUIRIDO");
 else return legado_main();
 return 0;
}
