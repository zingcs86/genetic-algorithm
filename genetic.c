#include <stdio.h>
#include <stdlib.h>

#define length 20
#define pop_num 20
#define generation_num 1
#define mutation_rate 0.005
#define crossover_rate 0.8
#define limit 300

char name[20][100] = { "#0ポッキー","#1うまい棒","#2じゃがりこ","#3ベビースターラーメン","#4チロルチョコ","#5かっぱえびせん","#6サッポロポテト","#7都こんぶ","#8ヨーグレットハイレモン","#9おにぎりせんべい","#10コアラのマーチ","#11チップスター","#12梅干しグミ","#13トッポ","#14アポロ","#15ハイレモン","#16さいころキャラメル","#17シゲキックス","#18ハイチュウ","#19ばなな", };
int price[20] = { 168,10,145,60,10,124,124,105,126,184,186,135,89,98,62,132,86,125,126,0 };
int cal[20] = { 496,45,325,347,61,486,446,22,110,475,520,325,221,322,152,216,322,105,221,210 };

// initialize gene
void init_gene( int gene[pop_num][length] ){
  int i,j;
  
  for(i=0;i<pop_num;i++){
    for(j=0;j<length;j++){
      if (rand()/ (double) RAND_MAX > 0.5 )
			gene[i][j] = 0;
      else
			gene[i][j] = 1;
    }
  }
}

// evaluation gene
void eval_gene( int gene[pop_num][length], double eval[pop_num] ) {
	int i, j;
	double p, c;

	// initialize
	for (i = 0; i < pop_num; i++) {
		p = 0.0;
		c = 0.0;

		for (j = 0; j < length; j++) {
			if (gene[i][j] == 1) {
				p += price[j];
				c += cal[i];
			}
		}

		if(p > limit) c -= 4.3 * (p - limit); // penalty for invalid solutions

		eval[i]	= c;
	}

	
}

// select new generation by roulette select
void select_gene( int gene[pop_num][length], double eval[pop_num] ){

	int new_gene[pop_num][length];
	double new_eval[pop_num];

	int i, j, t;
	double rnd, sum = 0.0;

	for (i = 0; i < pop_num; i++) {
		sum += eval[i];
	}

	// roulette select
	for (i = 0; i < pop_num; i++) {
		rnd = rand() / (double) RAND_MAX * sum;

		for (t = 0; t < pop_num; t++) {
			rnd -= eval[t];
			if (rnd <= 0.0)
				break;
		}

		for (j = 0; j < length; j++) {
			new_gene[i][j] = gene[i][j];
		}

		new_eval[i] = eval[t];
	}

	// copy
	for (i = 0; i < pop_num; i++) {
		for (j = 0; j < length; j++) {
			gene[i][j] = new_gene[i][j];
		}
	}

	eval[i] = new_eval[i];
}

void crossover_gene( int gene[pop_num][length] ){
	int i, j, pivot, w;

	for (i = 0; i < pop_num; i++) {
		if (i + 1 < pop_num && rand() / (double) RAND_MAX <= crossover_rate) {
			pivot = (int) (rand() / (double) RAND_MAX * length);

			for (j = pivot; j < length; j++) {
				w = gene[i][j];
				gene[i][j] = gene[i+1][j];
				gene[i+1][j] = w;
			}
		}
	} 
}

void mutation_gene( int gene[pop_num][length] ){
  
	int i, j;

	for (i = 0; i < pop_num; i++) {
		for (j = 0; j < length; j++) {
			if (rand() / (double) RAND_MAX <= mutation_rate) {
				gene[i][j] = 1 - gene[i][j];
			}
		}
	}

}

int main() {
	int generation, seed, i, j;
	int gene[pop_num][length];

	double eval[pop_num];
	double max, min, sum;

	int max_i;

	printf( "random seed:" );
	scanf( "%3d", &seed );
	srand( seed );

	init_gene( gene );
  	eval_gene( gene, eval );

  	for(generation = 0; generation < generation_num; generation++) {
  		printf( "generation:%d\n", generation );

  		max = min = -1.0;
    	sum = 0.0;

    	select_gene( gene, eval );
    	crossover_gene( gene );
 		mutation_gene( gene );

 		eval_gene( gene, eval );

 		for (i = 0; i < pop_num; i++) {
 			sum += eval[i];

 			if (max < 0.0 || max < eval[i])
 				max = eval[i], max_i = i;
 			if (min < 0.0 || min > eval[i])
 				min = eval[i];
 		}

 		// print gene
 		for (i = 0; i < pop_num; i++) {
			printf( "gene %d: ", i );

			for (j = 0; j < length; j++) {
				printf( "%d", gene[i][j] );
			}
			
			printf("  %lf\n", eval[i]);
		}

		printf("max: %lf, avg: %lf, min:%lf\n", max, sum / (double) pop_num, min);

		// pause. please remove if you need to speed up
    	// getchar();
  	}
	return 0;
}