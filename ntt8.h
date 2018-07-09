#include "swifft-avx2-8.h"

inline void bntt8(int x[16][8], vec Y[16][8]){
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	
	for(int row=0;row<16;row++){
		//printf("xbyte:%u\n",x[row][0]);
		//print(T_K0_I0[x[row][0]]);
		Y_temp1[0] = _mm256_mullo_epi32(M_K0_I0[0],T_K0_I0[x[row][0]]);	//ReduceY(&Y_temp1[0]);
		Y_temp1[1] = _mm256_mullo_epi32(M_K0_I0[1],T_K0_I0[x[row][1]]);	//ReduceY(&Y_temp1[1]);
		Y_temp1[2] = _mm256_mullo_epi32(M_K0_I0[2],T_K0_I0[x[row][2]]);	//ReduceY(&Y_temp1[2]);
		Y_temp1[3] = _mm256_mullo_epi32(M_K0_I0[3],T_K0_I0[x[row][3]]);	//ReduceY(&Y_temp1[3]);
		Y_temp1[4] = _mm256_mullo_epi32(M_K0_I0[4],T_K0_I0[x[row][4]]);	//ReduceY(&Y_temp1[4]);	
		Y_temp1[5] = _mm256_mullo_epi32(M_K0_I0[5],T_K0_I0[x[row][5]]);	//ReduceY(&Y_temp1[5]);
		Y_temp1[6] = _mm256_mullo_epi32(M_K0_I0[6],T_K0_I0[x[row][6]]);	//ReduceY(&Y_temp1[6]);	
		Y_temp1[7] = _mm256_mullo_epi32(M_K0_I0[7],T_K0_I0[x[row][7]]);	//ReduceY(&Y_temp1[7]);
		
		//column 0 and 4
		Y_temp2[0] = _mm256_add_epi32(Y_temp1[0],Y_temp1[2]);
		Y_temp2[1] = _mm256_add_epi32(Y_temp1[4],Y_temp1[6]);
		Y_temp2[2] = _mm256_add_epi32(Y_temp2[0],Y_temp2[1]);//y0 18

		Y_temp2[3] = _mm256_add_epi32(Y_temp1[1],Y_temp1[3]);
		Y_temp2[4] = _mm256_add_epi32(Y_temp1[5],Y_temp1[7]);
		Y_temp2[5] = _mm256_add_epi32(Y_temp2[3],Y_temp2[4]);//y1 18

		Y[row][0] = _mm256_add_epi32(Y_temp2[2],Y_temp2[5]);	ReduceY(&Y[row][0]);

		Y_temp2[6] = _mm256_add_epi32(Y_temp2[2],p257_19);
		//Y_temp2[6] = _mm256_add_epi32(Y_temp2[6],p257_19);
		Y[row][4] = _mm256_sub_epi32(Y_temp2[6],Y_temp2[5]);	ReduceY(&Y[row][4]);


		//column 1 and 5
		Y_temp3[0] = _mm256_add_epi32(Y_temp1[0],p257_17);
		Y_temp2[1] = _mm256_sub_epi32(Y_temp3[0],Y_temp1[4]);//y2
		

		Y_temp3[1] = _mm256_add_epi32(Y_temp1[2],p257_17);
		Y_temp3[1] = _mm256_sub_epi32(Y_temp3[1],Y_temp1[6]);
		Y_temp2[2] = _mm256_slli_epi32(Y_temp3[1],4);//y4
				
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[1],Y_temp2[2]);//22bits
	
		Y_temp3[3] = _mm256_add_epi32(Y_temp1[1],p257_17);
		Y_temp3[7] = _mm256_sub_epi32(Y_temp3[3],Y_temp1[5]);//y3
		
		Y_temp2[4] = _mm256_slli_epi32(Y_temp3[7],2);//19bits

		Y_temp3[4] = _mm256_add_epi32(Y_temp1[3],p257_17);
		Y_temp3[6] = _mm256_sub_epi32(Y_temp3[4],Y_temp1[7]);//y5 17bits
		Y_temp2[5] = _mm256_slli_epi32(Y_temp3[6],6);
		Y_temp2[6] = _mm256_add_epi32(Y_temp2[4],Y_temp2[5]);
		Y[row][1] = _mm256_add_epi32(Y_temp2[3],Y_temp2[6]);	ReduceY(&Y[row][1]);

		Y_temp3[5] = _mm256_add_epi32(Y_temp2[3],p257_23);
		Y[row][5] = _mm256_sub_epi32(Y_temp3[5],Y_temp2[6]);	
		//print(A[row][5]);
		ReduceY(&Y[row][5]);




		//column 3 and 7

		//Y_temp2[0] = _mm256_sub_epi32(Y_temp3[0],Y_temp1[4]);
		Y_temp2[1] = _mm256_add_epi32(Y_temp2[1],p257_21);
		Y_temp2[3] = _mm256_sub_epi32(Y_temp2[1],Y_temp2[2]);//y10
		
		Y_temp2[4] = _mm256_slli_epi32(Y_temp3[6],2);
		Y_temp2[5] = _mm256_slli_epi32(Y_temp3[7],6);
		Y_temp2[6] = _mm256_add_epi32(Y_temp2[4],Y_temp2[5]);//y11
					
		Y[row][3] = _mm256_add_epi32(Y_temp2[3],Y_temp2[6]);	ReduceY(&Y[row][3]);
		
		Y_temp3[5] = _mm256_add_epi32(Y_temp2[3],p257_23);	
		Y[row][7] = _mm256_sub_epi32(Y_temp3[5],Y_temp2[6]);	ReduceY(&Y[row][7]);

					

		//column 2 and 6
		Y_temp2[0] = _mm256_add_epi32(Y_temp1[0],Y_temp1[4]);
		Y_temp2[0] = _mm256_add_epi32(Y_temp2[0],p257_17);
		Y_temp2[1] = _mm256_add_epi32(Y_temp1[2],Y_temp1[6]);
		Y_temp2[2] = _mm256_sub_epi32(Y_temp2[0],Y_temp2[1]);//y6
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[2],p257_23);
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[3],p257_23);

		Y_temp2[4] = _mm256_add_epi32(Y_temp1[1],Y_temp1[5]);
		Y_temp2[4] = _mm256_add_epi32(Y_temp2[4],p257_17);
		Y_temp2[5] = _mm256_add_epi32(Y_temp1[3],Y_temp1[7]);
		Y_temp2[6] = _mm256_sub_epi32(Y_temp2[4],Y_temp2[5]);
		Y_temp2[7] = _mm256_slli_epi32(Y_temp2[6],4);//y7
		Y[row][2] = _mm256_add_epi32(Y_temp2[3],Y_temp2[7]);	ReduceY(&Y[row][2]);
		Y[row][6] = _mm256_sub_epi32(Y_temp2[3],Y_temp2[7]);	ReduceY(&Y[row][6]);
	}	
	
}

inline void gntt8(vec key[16][8], vec A[16][8]){
	vec t1, t2, temp[8];
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	
	for(int row=0;row<16;row++){
		for(int k0=0; k0<8; k0++){
			temp[k0] = zero;
			for(int k1=0; k1<8; k1++){
				t1 = _mm256_permutevar8x32_epi32 (key[row][k0], permute[k1]);
				t2 =  _mm256_mullo_epi32(k0i0[k1], t1);		ReduceY(&t2);
				temp[k0]  = _mm256_add_epi32(temp[k0],t2); 	ReduceY_final(&temp[k0]);
			}
		}
		//print(temp[0]);
		Y_temp1[0] = _mm256_mullo_epi32(M_K0_I0[0],temp[0]);	//ReduceY(&Y_temp1[0]);
		Y_temp1[1] = _mm256_mullo_epi32(M_K0_I0[1],temp[1]);	//ReduceY(&Y_temp1[1]);
		Y_temp1[2] = _mm256_mullo_epi32(M_K0_I0[2],temp[2]);	//ReduceY(&Y_temp1[2]);
		Y_temp1[3] = _mm256_mullo_epi32(M_K0_I0[3],temp[3]);	//ReduceY(&Y_temp1[3]);
		Y_temp1[4] = _mm256_mullo_epi32(M_K0_I0[4],temp[4]);	//ReduceY(&Y_temp1[4]);
		Y_temp1[5] = _mm256_mullo_epi32(M_K0_I0[5],temp[5]);	//ReduceY(&Y_temp1[5]);
		Y_temp1[6] = _mm256_mullo_epi32(M_K0_I0[6],temp[6]);	//ReduceY(&A_temp1[6]);
		Y_temp1[7] = _mm256_mullo_epi32(M_K0_I0[7],temp[7]);	//ReduceY(&A_temp1[7]);


		//Y_temp2[0] = _mm256_add_epi32(Y_temp1[0],Y_temp1[2]);
		//Y_temp2[1] = _mm256_add_epi32(Y_temp1[4],Y_temp1[6]);
		//Y_temp2[2] = _mm256_add_epi32(Y_temp2[0],Y_temp2[1]);//y0 18

		//Y_temp2[3] = _mm256_add_epi32(Y_temp1[1],Y_temp1[3]);
		//Y_temp2[4] = _mm256_add_epi32(Y_temp1[5],Y_temp1[7]);
		//Y_temp2[5] = _mm256_add_epi32(Y_temp2[3],Y_temp2[4]);//y1 18

		//Y[row][0] = _mm256_add_epi32(Y_temp2[2],Y_temp2[5]);	ReduceY(&Y[row][0]);

		//Y_temp2[6] = _mm256_add_epi32(Y_temp2[2],p257_19);
		//Y_temp2[6] = _mm256_add_epi32(Y_temp2[6],p257_19);
		//Y[row][4] = _mm256_sub_epi32(Y_temp2[6],Y_temp2[5]);	ReduceY(&Y[row][4]);


		
		//column 0 and 4
		Y_temp2[0] = _mm256_add_epi32(Y_temp1[0],Y_temp1[2]);
		Y_temp2[1] = _mm256_add_epi32(Y_temp1[4],Y_temp1[6]);
		Y_temp2[2] = _mm256_add_epi32(Y_temp2[0],Y_temp2[1]);//y0

		Y_temp2[3] = _mm256_add_epi32(Y_temp1[1],Y_temp1[3]);
		Y_temp2[4] = _mm256_add_epi32(Y_temp1[5],Y_temp1[7]);
		Y_temp2[5] = _mm256_add_epi32(Y_temp2[3],Y_temp2[4]);//y1

		A[row][0] = _mm256_add_epi32(Y_temp2[2],Y_temp2[5]);	ReduceY(&A[row][0]);

		Y_temp2[6] = _mm256_add_epi32(Y_temp2[2],p257_19);
		//Y_temp2[6] = _mm256_add_epi32(Y_temp2[6],p257_19);
		A[row][4] = _mm256_sub_epi32(Y_temp2[6],Y_temp2[5]);	ReduceY(&A[row][4]);


	
		//Y_temp3[0] = _mm256_add_epi32(Y_temp1[0],p257_17);
		//Y_temp2[1] = _mm256_sub_epi32(Y_temp3[0],Y_temp1[4]);//y2
		

		//Y_temp3[1] = _mm256_add_epi32(Y_temp1[2],p257_17);
		//Y_temp3[1] = _mm256_sub_epi32(Y_temp3[1],Y_temp1[6]);
		//Y_temp2[2] = _mm256_slli_epi32(Y_temp3[1],4);//y4
				
		//Y_temp2[3] = _mm256_add_epi32(Y_temp2[1],Y_temp2[2]);//22bits
	
		//Y_temp3[3] = _mm256_add_epi32(Y_temp1[1],p257_17);
		//Y_temp3[7] = _mm256_sub_epi32(Y_temp3[3],Y_temp1[5]);//y3
		
		//Y_temp2[4] = _mm256_slli_epi32(Y_temp3[7],2);//19bits

		//Y_temp3[4] = _mm256_add_epi32(Y_temp1[3],p257_17);
		//Y_temp3[6] = _mm256_sub_epi32(Y_temp3[4],Y_temp1[7]);//y5 17bits
		//Y_temp2[5] = _mm256_slli_epi32(Y_temp3[6],6);
		//Y_temp2[6] = _mm256_add_epi32(Y_temp2[4],Y_temp2[5]);
		//Y[row][1] = _mm256_add_epi32(Y_temp2[3],Y_temp2[6]);	ReduceY(&Y[row][1]);

		//Y_temp3[5] = _mm256_add_epi32(Y_temp2[3],p257_23);
		//Y[row][5] = _mm256_sub_epi32(Y_temp3[5],Y_temp2[6]);	
		//print(A[row][5]);
		//ReduceY(&Y[row][5]);


		//column 1 and 5
		Y_temp3[0] = _mm256_add_epi32(Y_temp1[0],p257_17);
		Y_temp2[1] = _mm256_sub_epi32(Y_temp3[0],Y_temp1[4]);//y2
		

		Y_temp3[1] = _mm256_add_epi32(Y_temp1[2],p257_17);
		Y_temp3[1] = _mm256_sub_epi32(Y_temp3[1],Y_temp1[6]);
		Y_temp2[2] = _mm256_slli_epi32(Y_temp3[1],4);//y4
				
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[1],Y_temp2[2]);//22bits
	
		Y_temp3[3] = _mm256_add_epi32(Y_temp1[1],p257_17);
		Y_temp3[7] = _mm256_sub_epi32(Y_temp3[3],Y_temp1[5]);//y3
		
		Y_temp2[4] = _mm256_slli_epi32(Y_temp3[7],2);//19bits

		Y_temp3[4] = _mm256_add_epi32(Y_temp1[3],p257_17);
		Y_temp3[6] = _mm256_sub_epi32(Y_temp3[4],Y_temp1[7]);//y5 17bits
		Y_temp2[5] = _mm256_slli_epi32(Y_temp3[6],6);
		Y_temp2[6] = _mm256_add_epi32(Y_temp2[4],Y_temp2[5]);
		A[row][1] = _mm256_add_epi32(Y_temp2[3],Y_temp2[6]);	ReduceY(&A[row][1]);

		Y_temp3[5] = _mm256_add_epi32(Y_temp2[3],p257_23);
		A[row][5] = _mm256_sub_epi32(Y_temp3[5],Y_temp2[6]);	
		//print(A[row][5]);
		ReduceY(&A[row][5]);




		//column 3 and 7

		//Y_temp2[0] = _mm256_sub_epi32(Y_temp3[0],Y_temp1[4]);
		Y_temp2[1] = _mm256_add_epi32(Y_temp2[1],p257_21);
		Y_temp2[3] = _mm256_sub_epi32(Y_temp2[1],Y_temp2[2]);//y10
		
		Y_temp2[4] = _mm256_slli_epi32(Y_temp3[6],2);
		Y_temp2[5] = _mm256_slli_epi32(Y_temp3[7],6);
		Y_temp2[6] = _mm256_add_epi32(Y_temp2[4],Y_temp2[5]);//y11
					
		A[row][3] = _mm256_add_epi32(Y_temp2[3],Y_temp2[6]);	ReduceY(&A[row][3]);
		
		Y_temp3[5] = _mm256_add_epi32(Y_temp2[3],p257_23);	
		A[row][7] = _mm256_sub_epi32(Y_temp3[5],Y_temp2[6]);	ReduceY(&A[row][7]);

					

		//column 2 and 6
		Y_temp2[0] = _mm256_add_epi32(Y_temp1[0],Y_temp1[4]);
		Y_temp2[0] = _mm256_add_epi32(Y_temp2[0],p257_19);
		Y_temp2[1] = _mm256_add_epi32(Y_temp1[2],Y_temp1[6]);
		Y_temp2[2] = _mm256_sub_epi32(Y_temp2[0],Y_temp2[1]);//y6
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[2],p257_23);
		Y_temp2[3] = _mm256_add_epi32(Y_temp2[3],p257_23);

		Y_temp2[4] = _mm256_add_epi32(Y_temp1[1],Y_temp1[5]);
		Y_temp2[4] = _mm256_add_epi32(Y_temp2[4],p257_17);
		Y_temp2[5] = _mm256_add_epi32(Y_temp1[3],Y_temp1[7]);
		Y_temp2[6] = _mm256_sub_epi32(Y_temp2[4],Y_temp2[5]);
		Y_temp2[7] = _mm256_slli_epi32(Y_temp2[6],4);//y7
		A[row][2] = _mm256_add_epi32(Y_temp2[3],Y_temp2[7]);	ReduceY(&A[row][2]);
		A[row][6] = _mm256_sub_epi32(Y_temp2[3],Y_temp2[7]);	ReduceY(&A[row][6]);
	}	
}

