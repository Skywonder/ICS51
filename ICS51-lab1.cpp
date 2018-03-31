#include <stdio.h>
#include <ctype.h>
#include<stdlib.h>

/*
ICS 51, Lab #1

-----------------------------------------------------------------

PART I:

Given a 32-bit integer, swap all odd bits with even bits.
For example, if the given number is 23 (00010111), it should be
converted to 43 (00101011). Every even position bit is swapped
with adjacent bit on right side, and every odd position bit is
swapped with adjacent on left side.

Implementation details:
The input integer is stored in registers ebx. You need to store the
answer into register eax.

Hint:
You only need a half-dozen instructions to do this.
Think about "clearing" odd (even) bits and using shift (shl, shr)
instructions.

If we have an 8-bit number 00010111, the expected output is 00101011.
Clearing (zeroing) odd bits (7, 5, 3, 1) of 00010111 would make it
00010101. Now if we shift it to the left by one bit, it becomes
00101010. The odd bits of 00101010 are equal to the odd bits of the
expected output. They are "0 1 1 1".

Using the same technique, generate the even bits of the expected
output and then figure out a way for combining generated odd and
even bits.

-----------------------------------------------------------------

PART II:

You are given three integers. You need to add the smallest
number to the largest number and multiply that sum by two. You
cannot use mul/imul instructions.

Implementation details:
The three integers are stored in registers eax, ebx, and ecx. You
need to store the answer into register eax.

Hint:
One way of finding minimum and maximum number is to sort the numbers.

-----------------------------------------------------------------

PART III:

This function takes in a square sized grayscale image and applies
thresholding to each pixel.

Width and height of the image are equal to "dim".

-----------------------------------------------------------------


IMPORTATNT NOTES:

- To test your code see the "testcode" function in lab1-testing.c

- Write your assembly code only in the marked blocks.

- Do NOT change anything outside the marked blocks.

- For part II, you are NOT allowed to use any version of the
MULTIPLY instruction!

- Remember to fill in your name, student ID below.

- This lab has to be done individually.

- Submit ONLY lab1.c file for grading.

*/

char *yourName = "Kuan-Ping Chang";
char *yourStudentID = "71018021";

/* Implement the body of this function for part I*/
int swapBits(int x)
{
	int result;

	__asm
	{
		mov ebx, x //input stored at ebx

				   // YOUR CODE STARTS HERE

			mov eax, ebx
			and eax, 0xAAAAAAAA
			shr eax, 1
			mov ecx, ebx
			and ecx, 0x55555555
			shl ecx, 1
			add eax, ecx
			// YOUR CODE ENDS HERE

			mov result, eax //end result should be here
	}
	return result;
}

/* Implement the body of this function for part II*/
void doubleMax(int a, int b, int c, int *result)
{
	__asm
	{

		mov esi, result

		mov eax, a
		mov ebx, b
		mov ecx, c

			// YOUR CODE STARTS HERE
		mov edx, eax
		cmp ebx, edx //compare a and b
		jle LesserValue //jump if a is greater than b
		mov edx, ebx

		LesserValue://Find larger of a, b, c; store max in edx
			cmp ecx, edx
			jle LesserValue2
			mov edx, ecx

		LesserValue2:
			cmp ebx, eax
			jge GreaterValue
			mov eax, ebx
		
		GreaterValue:
			cmp ecx, eax
			jge GreaterValue2
			mov eax, ecx

		GreaterValue2:
			add eax, edx
			shl eax, 1



		// YOUR CODE ENDS HERE

		mov[esi][0], eax
	}

	return;
}


/* Implement the body of this function for part III*/
void imageThresholding(unsigned char* image, int dim) {

	__asm
	{
		mov eax, image
		mov ebx, dim

			// YOUR CODE STARTS HERE
			//object is square
	
			mov edi, 0 //set start for the row, increment by 1 for next value
			mov ebx, 0
			BEGIN_ROW: //start at 0
				cmp edi, dim //compare the ecx vs ebx position (value)
				jge END_ROW //if greater or equal it meant we reach the end roll
				mov esi, 0//otherwise check the col at position 0

			BEGIN_COL : //start at 0
				cmp esi, dim// compare col position with dimension 
				jge END_COL//if greater or equal then end the column
				mov ebx, 0//otherwise start at 0
				mov edx, 0

			Begin_MUL :
				cmp edx, dim
				jge END_MUL
				add ebx, edi
				inc edx
				jmp BEGIN_MUL
			END_MUL:
				add ebx, esi
				xor edx, edx
				mov dl, [eax + ebx]
				mov cl, 0x80// 0x80 //set cl to value 0
				and cl, dl
				cmp cl, 0x00//0x00//if higher than 128
				jne IF_HIGHER
				xor edx, edx//else
				jmp NEXT

			IF_HIGHER:
				or edx, 0xFF //change to 255

			NEXT:
				mov [eax + ebx], dl //assign new value
				inc esi //increment to the next column
 				jmp BEGIN_COL
			END_COL:
				inc edi
				jmp BEGIN_ROW
			END_ROW:
				mov image, eax


				// YOUR CODE ENDS HERE    

	}
}

