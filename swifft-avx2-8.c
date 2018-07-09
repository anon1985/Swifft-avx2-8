#include "ntt8.h"

vec A[16][8] align;
vec Y[16][8] align;
vec YA[16][8] align;
vec op[8] align;
int opi[8][8] align;
vec X[16][8] align;

/*======================*/

int x[16][8];
int a[16][64];


void read_Input(){
/*	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<8;j++){
			scanf("%d",&(x[i][j]));
		}
	}
	for(i=0;i<16;i++){
		for(j=0;j<64;j++){
			scanf("%d",&(a[i][j]));
		}
	}
*/
}
void parse(int xbyte[16][8], u8 x[sklen]){
	int i,j,k,row_t;

	for(int row=0;row<16;row++){
		row_t = row*8;
		for(i=0; i<8; i++){
			xbyte[row][i] = x[row_t+i];
		}
	}

}


void set_Key(){
	int y[16][64];
	int i,k,j;
	int t;
	int om_odd_pow[64];
	vec om_base[8], om_temp[8][8], key[16][8],temp[8];
	vec tk0i0k[8];
	vec t1,t2;
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	
	for(int row=0;row<16;row++){
		for(int k0=0; k0 < 8; k0++){
			0[(u32 *) &(key[row][k0])] = a[row][k0+8*0];
			1[(u32 *) &(key[row][k0])] = a[row][k0+8*1];
			2[(u32 *) &(key[row][k0])] = a[row][k0+8*2];
			3[(u32 *) &(key[row][k0])] = a[row][k0+8*3];
			4[(u32 *) &(key[row][k0])] = a[row][k0+8*4];
			5[(u32 *) &(key[row][k0])] = a[row][k0+8*5];
			6[(u32 *) &(key[row][k0])] = a[row][k0+8*6];
			7[(u32 *) &(key[row][k0])] = a[row][k0+8*7];
		}
	}
	
	gntt8(key, A);
	for(int row=0;row<16;row++){
		ReduceY_final(&A[row][0]);ReduceY_final(&A[row][1]);ReduceY_final(&A[row][2]);ReduceY_final(&A[row][3]);
		ReduceY_final(&A[row][4]);ReduceY_final(&A[row][5]);ReduceY_final(&A[row][6]);ReduceY_final(&A[row][7]);
	}
}	
void print(vec Y){
	u32 pk[8];
	pk[0]  = 0[(u32 *) &Y];
   	pk[1]  = 1[(u32 *) &Y];
    	pk[2]  = 2[(u32 *) &Y];
    	pk[3]  = 3[(u32 *) &Y];
    	pk[4]  = 4[(u32 *) &Y];
   	pk[5]  = 5[(u32 *) &Y];
    	pk[6]  = 6[(u32 *) &Y];
    	pk[7]  = 7[(u32 *) &Y];
	printf("\n");
	for(int i0=0;i0<8;i0++)
		printf("%u ",pk[i0]);
	printf("\n");
}
void SWIFFT(int x[16][8], u8 *pk){
	int i1;
	int k0;
	vec Yt[8];
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	vec Y[16][8];
	int t;
	vec op_temp[8];
	vec mk0i0, poi1k0;
	int xbyte[8];
	bntt8(x, Y);
	
	for(int i=0; i<8; i++){
		Y[0][i]  = _mm256_mullo_epi32(Y[0][i], A[0][i]);	//ReduceY(&(Y[0][i]));
		Y[1][i]  = _mm256_mullo_epi32(Y[1][i], A[1][i]);	//ReduceY(&(Y[1][i]));
		Y[2][i]  = _mm256_mullo_epi32(Y[2][i], A[2][i]);	//ReduceY(&(Y[2][i]));
		Y[3][i]  = _mm256_mullo_epi32(Y[3][i], A[3][i]);	//ReduceY(&(Y[3][i]));
		Y[4][i]  = _mm256_mullo_epi32(Y[4][i], A[4][i]);	//ReduceY(&(Y[4][i]));
		Y[5][i]  = _mm256_mullo_epi32(Y[5][i], A[5][i]);	//ReduceY(&(Y[5][i]));
		Y[6][i]  = _mm256_mullo_epi32(Y[6][i], A[6][i]);	//ReduceY(&(Y[6][i]));
		Y[7][i]  = _mm256_mullo_epi32(Y[7][i], A[7][i]);	//ReduceY(&(Y[7][i]));
		Y[8][i]  = _mm256_mullo_epi32(Y[8][i], A[8][i]);	//ReduceY(&(Y[8][i]));
		Y[9][i]  = _mm256_mullo_epi32(Y[9][i], A[9][i]);	//ReduceY(&(Y[9][i]));
		Y[10][i] = _mm256_mullo_epi32(Y[10][i],A[10][i]);	//ReduceY(&(Y[10][i]));
		Y[11][i] = _mm256_mullo_epi32(Y[11][i],A[11][i]);	//ReduceY(&(Y[11][i]));
		Y[12][i] = _mm256_mullo_epi32(Y[12][i],A[12][i]);	//ReduceY(&(Y[12][i]));
		Y[13][i] = _mm256_mullo_epi32(Y[13][i],A[13][i]);	//ReduceY(&(Y[13][i]));
		Y[14][i] = _mm256_mullo_epi32(Y[14][i],A[14][i]);	//ReduceY(&(Y[16][i]));
		Y[15][i] = _mm256_mullo_epi32(Y[15][i],A[15][i]);	//ReduceY(&(Y[15][i]));
		
		op_temp[0] = _mm256_add_epi32(Y[0][i],Y[1][i]);   
		op_temp[1] = _mm256_add_epi32(Y[2][i],Y[3][i]); 
		op_temp[2] = _mm256_add_epi32(Y[4][i],Y[5][i]);   
		op_temp[3] = _mm256_add_epi32(Y[6][i],Y[7][i]);
		op_temp[4] = _mm256_add_epi32(Y[8][i],Y[9][i]);   
		op_temp[5] = _mm256_add_epi32(Y[10][i],Y[11][i]); 
		op_temp[6] = _mm256_add_epi32(Y[12][i],Y[13][i]); 
		op_temp[7] = _mm256_add_epi32(Y[14][i],Y[15][i]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[1]); 
		op_temp[2] = _mm256_add_epi32(op_temp[2],op_temp[3]); 
		op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[5]); 
		op_temp[6] = _mm256_add_epi32(op_temp[6],op_temp[7]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[2]); 
		op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[6]); 

		op[i] = _mm256_add_epi32(op_temp[0],op_temp[4]);
		ReduceY_final(&op[i]);
	}
	unpackY(op,pk);

}

void gSWIFFT(int x[16][64],u32 *pk){
	int i1;
	int k0;
	vec Yt[8];
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	int t;
	vec op_temp[8];
	vec mk0i0, poi1k0;

	for(int row=0;row<16;row++){
		for(int k0=0; k0 < 8; k0++){
			0[(u32 *) &(X[row][k0])] = x[row][8*k0+0];
			1[(u32 *) &(X[row][k0])] = x[row][8*k0+1];
			2[(u32 *) &(X[row][k0])] = x[row][8*k0+2];
			3[(u32 *) &(X[row][k0])] = x[row][8*k0+3];
			4[(u32 *) &(X[row][k0])] = x[row][8*k0+4];
			5[(u32 *) &(X[row][k0])] = x[row][8*k0+5];
			6[(u32 *) &(X[row][k0])] = x[row][8*k0+6];
			7[(u32 *) &(X[row][k0])] = x[row][8*k0+7];
		}
	}
	gntt8(X, Y);
	

	for(int i=0; i<8; i++){
		Y[0][i]  = _mm256_mullo_epi32(Y[0][i], A[0][i]);	//ReduceY(&Y[0][i]);
		Y[1][i]  = _mm256_mullo_epi32(Y[1][i], A[1][i]);	//ReduceY(&Y[1][i]);
		Y[2][i]  = _mm256_mullo_epi32(Y[2][i], A[2][i]);	//ReduceY(&Y[2][i]);
		Y[3][i]  = _mm256_mullo_epi32(Y[3][i], A[3][i]);	//ReduceY(&Y[3][i]);
		Y[4][i]  = _mm256_mullo_epi32(Y[4][i], A[4][i]);	//ReduceY(&Y[4][i]);
		Y[5][i]  = _mm256_mullo_epi32(Y[5][i], A[5][i]);	//ReduceY(&Y[5][i]);
		Y[6][i]  = _mm256_mullo_epi32(Y[6][i], A[6][i]);	//ReduceY(&Y[6][i]);
		Y[7][i]  = _mm256_mullo_epi32(Y[7][i], A[7][i]);	//ReduceY(&Y[7][i]);
		Y[8][i]  = _mm256_mullo_epi32(Y[8][i], A[8][i]);	//ReduceY(&Y[8][i]);
		Y[9][i]  = _mm256_mullo_epi32(Y[9][i], A[9][i]);	//ReduceY(&Y[9][i]);
		Y[10][i] = _mm256_mullo_epi32(Y[10][i],A[10][i]);	//ReduceY(&Y[10][i]);
		Y[11][i] = _mm256_mullo_epi32(Y[11][i],A[11][i]);	//ReduceY(&Y[11][i]);
		Y[12][i] = _mm256_mullo_epi32(Y[12][i],A[12][i]);	//ReduceY(&Y[12][i]);
		Y[13][i] = _mm256_mullo_epi32(Y[13][i],A[13][i]);	//ReduceY(&Y[13][i]);
		Y[14][i] = _mm256_mullo_epi32(Y[14][i],A[14][i]);	//ReduceY(&Y[14][i]);
		Y[15][i] = _mm256_mullo_epi32(Y[15][i],A[15][i]);	//ReduceY(&Y[15][i]);
		
		op_temp[0] = _mm256_add_epi32(Y[0][i],Y[1][i]);   
		op_temp[1] = _mm256_add_epi32(Y[2][i],Y[3][i]); 
		op_temp[2] = _mm256_add_epi32(Y[4][i],Y[5][i]);   
		op_temp[3] = _mm256_add_epi32(Y[6][i],Y[7][i]);
		op_temp[4] = _mm256_add_epi32(Y[8][i],Y[9][i]);   
		op_temp[5] = _mm256_add_epi32(Y[10][i],Y[11][i]); 
		op_temp[6] = _mm256_add_epi32(Y[12][i],Y[13][i]); 
		op_temp[7] = _mm256_add_epi32(Y[14][i],Y[15][i]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[1]); 
		op_temp[2] = _mm256_add_epi32(op_temp[2],op_temp[3]); 
		op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[5]); 
		op_temp[6] = _mm256_add_epi32(op_temp[6],op_temp[7]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[2]); 
		op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[6]); 

		op[i] = _mm256_add_epi32(op_temp[0],op_temp[4]);	ReduceY_final(&op[i]);
	}

	//print(op[0]);
	unpack_rgY(op,pk);

}
/*
void ReduceY(vec *Y){
	vec a0,a1;
	a0 = _mm256_and_si256(*Y,mask255); a1 = _mm256_srli_epi32(*Y,8); *Y = _mm256_sub_epi32(a0,a1);
	a0 = _mm256_cmpgt_epi32(*Y,allone);//print(*Y);
	a0 = _mm256_xor_si256(a0,allone); a0 = _mm256_and_si256(a0,p257);
	*Y = _mm256_add_epi32(*Y,a0);//print(*Y);
}*/

void ReduceY(vec *Y){
	vec a0,a1;
	a0 = _mm256_and_si256(*Y,mask16b); a1 = _mm256_srli_epi32(*Y,16); *Y = _mm256_add_epi32(a0,a1);
	//a0 = _mm256_and_si256(*Y,mask16b); a1 = _mm256_srli_epi32(*Y,16); *Y = _mm256_add_epi32(a0,a1);
}


void ReduceY_final(vec *Y){
	vec a0,a1;
	a0 = _mm256_and_si256(*Y,mask16b); a1 = _mm256_srli_epi32(*Y,16); *Y = _mm256_add_epi32(a0,a1);
	a0 = _mm256_and_si256(*Y,mask16b); a1 = _mm256_srli_epi32(*Y,16); *Y = _mm256_add_epi32(a0,a1);
	a0 = _mm256_and_si256(*Y,mask255); a1 = _mm256_srli_epi32(*Y,8); *Y = _mm256_sub_epi32(a0,a1);
	a0 = _mm256_cmpgt_epi32(*Y,allone);//print(*Y);
	a0 = _mm256_xor_si256(a0,allone); a0 = _mm256_and_si256(a0,p257);
	*Y = _mm256_add_epi32(*Y,a0);//print(*Y);
}

inline void outputY(u32 Y[8][8]){
	printf("\n\n\n\n");
	for(int i1=0;i1<8;i1++){
		for(int i0=0;i0<8;i0++)
			printf("%d ",Y[i1][i0]);
	}
	printf("\n\n\n\n");
}


inline void unpack_rgY(vec Y[8],u32 *pk){
	int i,j;
	//u32 opi[4][16];
	for(i=0;i<8;i++){
	    	pk[8*i+0]  = 0[(u32 *) &Y[i]]%257;
	   	pk[8*i+1]  = 1[(u32 *) &Y[i]]%257;
	    	pk[8*i+2]  = 2[(u32 *) &Y[i]]%257;
	    	pk[8*i+3]  = 3[(u32 *) &Y[i]]%257;
	    	pk[8*i+4]  = 4[(u32 *) &Y[i]]%257;
	   	pk[8*i+5]  = 5[(u32 *) &Y[i]]%257;
	    	pk[8*i+6]  = 6[(u32 *) &Y[i]]%257;
	    	pk[8*i+7]  = 7[(u32 *) &Y[i]]%257;
	}
	
	
}


inline void unpackY(vec Y[8],u8 *pk){
	int i,j;
	u32 opi[8];
	for(i=0;i<8;i++){
	    	opi[0]  = 0[(u32 *) &Y[i]]%257;
	   	opi[1]  = 1[(u32 *) &Y[i]]%257;
	    	opi[2]  = 2[(u32 *) &Y[i]]%257;
	    	opi[3]  = 3[(u32 *) &Y[i]]%257;
	    	opi[4]  = 4[(u32 *) &Y[i]]%257;
	   	opi[5]  = 5[(u32 *) &Y[i]]%257;
	    	opi[6]  = 6[(u32 *) &Y[i]]%257;
	    	opi[7]  = 7[(u32 *) &Y[i]]%257;
	
		pk[i*9+0] = (u8)(0xFF & opi[0]);

		pk[i*9+1] = (u8)(0x1 & (opi[0]>>8));
		pk[i*9+1] = pk[i*9+1] | (u8)((0x7F&opi[1])<<1);

		pk[i*9+2] = (u8)(0x3 & (opi[1]>>7));
		pk[i*9+2] = pk[i*9+2] | (u8)((0x3F&opi[2])<<2);

		pk[i*9+3] = (u8)(0x7 & (opi[2]>>6));
		pk[i*9+3] = pk[i*9+3] | (u8)((0x1F&opi[3])<<3);

		pk[i*9+4] = (u8)(0xF & (opi[3]>>5));
		pk[i*9+4] = pk[i*9+4] | (u8)((0xF&opi[4])<<4);

		pk[i*9+5] = (u8)(0x1F & (opi[4]>>4));
		pk[i*9+5] = pk[i*9+5] | (u8)((0x7&opi[5])<<5);

		pk[i*9+6] = (u8)(0x3F & (opi[5]>>3));
		pk[i*9+6] = pk[i*9+6] | (u8)((0x3&opi[6])<<6);

		pk[i*9+7] = (u8)(0x7F & (opi[6]>>2));
		pk[i*9+7] = pk[i*9+7] | (u8)((0x1&opi[7])<<7);

		pk[i*9+8] = (u8)(0xFF & (opi[7]>>1));

	}
	
	
}



