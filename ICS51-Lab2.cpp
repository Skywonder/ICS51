/*
	ICS 51. Lab #3

	IMPORTATNT NOTES:

	- Noramally, the compiler adds the prologue/epilogue code for the callee. However, when the 
	__declspec(naked) attribute is used, it is the programmer's responsibility to write the 
	prologue/epilogue code. In this lab you need to write prologue/epilogue code sections only 
	for the functions that have been declared with the naked attribute, namely:

	   + findMinIndex
	   + recFactorial
	   + performOperation
	   + fibonacci

	- You cannot define any local variable in the C code. If you need additional variables in your
	assembly code, you have to allocate space for them in the prologue of the function. 

	- You are not allowed to change anything in this file except for adding your assembly code 
    between the lines marked "BEGIN YOUR CODE HERE" and "END YOUR CODE HERE".

	- Remember to fill in your name, student ID below.

	- ONLY submit the lab3.c file for grading.

*/

char *yourName = "Kuan-Ping Chang";
char *yourStudentID = "71018021";

/***********************************************************************************************

	PART 1: Selection Sort

	Selection sort is an in-place comparison sort algorithm that works by dividing the input list 
	into two parts: the sublist of items already sorted, which is built up from left to right of 
	the list, and the sublist of items remaining to be sorted that occupy the rest of the list. 
	Initially, the sorted sublist is empty and the unsorted sublist is the entire input list. 
	The algorithm proceeds by finding the smallest element in the unsorted sublist, exchanging it 
	with the leftmost unsorted element (putting it in sorted order), and moving the sublist 
	boundaries one element to the right. To learn more, read: 
	https://en.wikipedia.org/wiki/Selection_sort#Example

	Our implementation here uses a function called "findMinIndex" to find the index of smallest
	element between ith index and jth index of the array. The function "selectionSort" uses
	this function to find the smallest number and exchanges it with the leftmost unsorted element
	in each iteration. You need to implement the behavior of both functions in x86 assembly.

************************************************************************************************/

__declspec(naked) 
int findMinIndex (int integer_array[], int i, int j)
{

// C code to be converted to x86 assembly
/*
    int iMin = i;
    // test against elements after i and before j to find the smallest 
    for ( i ; i < j; i++) {
        // if this element is less, then it is the new minimum   
        if (integer_array[i] < integer_array[iMin]) {
            // found new minimum; remember its index 
            iMin = i;
        }
    }

    return iMin;
*/

	__asm{
			// BEGIN YOUR CODE HERE
		push ebp;
		mov ebp, esp;
		push edi;
		push esi;
		push ebx;
		mov eax, [ebp + 8];
		mov ebx, [ebp + 12];
		mov ecx, [ebp + 16];
		mov edx, ebx;
	FOR_BEGIN:
		cmp ebx, ecx;
		jge FOR_END;
		mov esi, [eax + 4 * ebx];
		mov edi, [eax + 4 * edx];
		cmp esi, edi;
		jge FOR_CONT;
		mov edx, ebx;

	FOR_CONT: //increment the count
		inc ebx;
		jmp FOR_BEGIN;
	FOR_END:
		mov eax, edx;
		pop ebx;
		pop esi;
		pop edi;
		mov esp, ebp;
		pop ebp;

			// END YOUR CODE HERE
			ret
	}

}

void selectionSort (int integer_array[], int array_size)
{

// C code to be converted to x86 assembly
/*  
	int j;
	int iMin;
	int temp;

	// advance the position through the entire array //
	// (could do j < n-1 because single element is also min element) //
	for (j = 0; j < array_size-1; j++) {

		// find the index of min element in the unsorted a[j .. n-1] //
 		iMin = findMinIndex (integer_array, j, array_size);
 
		if(iMin != j) { // swap values
			temp = integer_array[iMin];
			integer_array[iMin] = integer_array [j];
			integer_array[j] = temp;	
		}
	}
*/

	__asm{
			// BEGIN YOUR CODE HERE
		mov esi, integer_array;
		mov edx, array_size;
		sub edx, 1;
		xor edi, edi;
	FOR_BEGIN:
		cmp edi, edx;
		jge FOR_END;
		push eax;
		push ecx;
		push edx;
		mov ebx, array_size;
		push ebx;
		push edi;
		push esi;
		call findMinIndex;
		add esp, 12;
		mov ebx, eax;
		pop edx;
		pop ecx;
		pop eax;
		cmp ebx, edi;
		je FOR_CONTINUE;
		mov eax, [esi + 4 * ebx];
		mov ecx, [esi + 4 * edi];
		xor eax, ecx;
		xor ecx, eax;
		xor eax, ecx;
		mov[esi + 4 * ebx], eax;
		mov[esi + 4 * edi], ecx;
	FOR_CONTINUE:
		inc edi;
		jmp FOR_BEGIN;
	FOR_END:
		mov integer_array, esi;



			// END YOUR CODE HERE
	}

}

/***********************************************************************************************

    PART 2: Recursive Factorial

	Implement two functions that calculates the factorial of an integer not greater than 12.

	The 1st function, factorial (unsigned int input, unsigned int *output), is the non 
	recursive wrapper function which takes the input, sets up the stack, calls the recursive 
	function and finally puts the result in the location pointed to by the pointer argument called
    output.

	The 2nd function, recFactorial(unsigned int n), is the recursive function that actually 
	calculates the factorial.

	Given an unsigned integer number n,we define FACTORIAL(n) as:

        FACTORIAL(n) =  1                   ,    if n = 0 or n = 1
                        n * FACTORIAL(n-1)  ,    if n > 1
		
	Tip:
		
	- You need to use IMUL instruction to multiply unsigned integers. Learn how to use it:
	http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#instructions

************************************************************************************************/

__declspec(naked) 
unsigned int recFactorial (unsigned int n)
{

// C code to be converted to x86 assembly
/*
	if (n<=1) 
		return 1;
	else 
		return n * recFactorial(n-1); 
*/

	__asm{
			// BEGIN YOUR CODE HERE
		push ebp;
		mov ebp, esp;
		push edi;
		push esi;
		push ebx;
		mov ebx, [ebp + 8];
		cmp ebx, 1;
		ja RECURSIVE_LOOP;
		mov eax, 1;
		jmp END;
	RECURSIVE_LOOP:
		push eax;
		push ecx;
		push edx;
		dec ebx;
		push ebx;
		call recFactorial;
		add esp, 4;
		mov ebx, eax;
		pop edx;
		pop ecx;
		pop eax;
		imul ebx, [ebp + 8];
		mov eax, ebx;
	END:
		pop ebx;
		pop esi;
		pop edi;
		mov esp, ebp;
		pop ebp;

			// END YOUR CODE HERE
			ret
	}

}

void factorial (unsigned int input, unsigned int *output)
{

// C code to be converted to x86 assembly
/*
	*output = recFactorial (input);
*/

	__asm{
			// BEGIN YOUR CODE HERE
		//push onto stack
		push eax;
		push ecx;
		push edx;

		//accept parameter
		mov eax, input;

		//push the new eax onto stack
		push eax;

		//that stack is called in recFactorial
		call recFactorial;

		add esp, 4; 

		//move output to ebx, then mov eax to address of [ebx]
		mov ebx, output;
		mov[ebx], eax; 

		//clean up stack
		pop edx;
		pop ecx;
		pop eax;


			// END YOUR CODE HERE
	}

}

/***********************************************************************************************

    PART 3: Vector Operation

	The vectorOperations function gets as parameters 4 arrays of the same length and
	the number of elements in those arrays. 

	For each pair of elements in first_array and second_array, it calls performOperation
	and stores the returned result in the same index of output_array.

	output_array[index] = performOperation(operation_array[index], first_array[index], second_array[index]).

	You also need to implement the body of performOperation function according to the provided
	C code.
  
************************************************************************************************/

__declspec(naked) 
int performOperation (int operation, int first, int second)
{

// C code to be converted to x86 assembly
/*
	if (operation==0) 
		return first + second;
	else if (operation==1)
		return first - second;
	else if (operation==2)
		return (first > second)? first : second;
	else if (operation==3)
		return (first < second)? first : second;
	else
		return first;
*/

	__asm {
			// BEGIN YOUR CODE HERE
		push ebp;
		mov ebp, esp;

		sub esp, 4; //get memory space
		//push onto stack
		push edi;
		push esi;
		push ebx;

		mov ebx, [ebp + 8];
		mov ecx, [ebp + 12];
		mov edx, [ebp + 16];

		cmp ebx, 0; //first operation return first + second
		jne IF_Operation1;

		mov[ebp - 4], ecx;
		add[ebp - 4], edx;
		mov eax, [ebp - 4];
		jmp CONTINUE;
		//second operation first - second
	IF_Operation1:
		cmp ebx, 1;
		jne IF_Operation2;

		mov[ebp - 4], ecx;
		sub[ebp - 4], edx;
		mov eax, [ebp - 4];
		jmp CONTINUE;
		//third operation if first > second 
	IF_Operation2:
		cmp ebx, 2;
		jne IF_Operation3;

		cmp ecx, edx;
		jle IF_Operation2B;

		mov eax, ecx;
		jmp CONTINUE;

	IF_Operation2B:
		mov eax, edx;
		jmp CONTINUE;

		//fourth operation if first < second
	IF_Operation3:
		cmp ebx, 3;
		jne ELSE;

		cmp ecx, edx;
		jge IF_Operation3B;//if edx is less

		mov eax, ecx; //if ecx is less 
		jmp CONTINUE;

	IF_Operation3B:
		mov eax, edx; 
		jmp CONTINUE;
	ELSE:
		mov eax, ecx;

		//end remove stack, deallocate space 
	CONTINUE:
		pop ebx;
		pop esi;
		pop edi;

		mov esp, ebp;

		pop ebp;


			// END YOUR CODE HERE
			ret
	}
  
}

void vectorOperations (int number_of_elements, int *first_array, int *second_array, 
					   int *operation_array, int *output_array)
{
// C code to be converted to x86 assembly
/*
	int i;
	for (i=0; i<number_of_elements; i++)
	{
		output_array[i] = performOperation(operation_array[i], first_array[i], second_array[i]);
	}
*/

	__asm {
			// BEGIN YOUR CODE HERE
		mov edi, output_array; //move array into edi
		xor esi, esi;
	BEGIN_LOOP:
		cmp esi, number_of_elements; //should be 0 at start
		jge END_LOOP;

		//push eax, ecx, edx onto stack
		push eax;
		push ecx;
		push edx;

		//move array into stack 
		mov eax, second_array;
		mov eax, [eax + 4 * esi]; //get the value onto eax
		mov ebx, first_array;
		mov ebx, [ebx + 4 * esi];
		mov ecx, operation_array;
		mov ecx, [ecx + 4 * esi];

		//push the value onto eax 
		push eax;
		push ebx;
		push ecx;

		call performOperation;

		add esp, 12;
		//store the value inside edi at position edi + 4 * edi (index) space memory 
		mov[edi + 4 * esi], eax;
		//reset stack 
		pop edx;
		pop ecx;
		pop eax;

		inc esi; //increase index and move to next value 
		jmp BEGIN_LOOP;
	END_LOOP:
		mov output_array, edi;


			// END YOUR CODE HERE
	}
}

/***********************************************************************************************

    PART 4: Change Letter Case

	You are given an array of characters (a string) that may contain numbers, upper and	lower case 
    letters, punctuations symbols, etc. Implement a function to convert all uppercase letters to 
    lowercase and vice versa. 
    For example "ThisIsTheFinalLabOf51" should be converted to "tHISiStHEfINALlABoF51". 
	The function has the following argument: An array of charaters and the size of the array. The 
	array should be changed in place. There is no explicit output for this function.

	Tips:

	- Note that each element of the arrays is a char (1 byte). You can NOT use a 32-bit register 
	to read/write from/to a memory location. Instead you should use the 8-bit version of the x86
	general-purpose registers. 
	More info: http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#registers

	- The link to the ascii codes is here: http://www.asciitable.com/

************************************************************************************************/

void changeCase (char char_array[], int array_size ) {

	__asm{
			// BEGIN YOUR CODE HERE
		mov edx, char_array;

		xor esi, esi; //reset esi
	BEGIN_LOOP:
		cmp esi, array_size; //compare esi size...esi should be zero
		jge END_LOOP; //end the loop is esi is greater

		mov al, byte ptr[edx + esi]; //
		cmp al, 65;
		jl SECOND;

		cmp al, 90;
		jg SECOND;
		//start on uppercase
		or al, 32;
		mov byte ptr[edx + esi], al;
		jmp CONTINUE;
	SECOND:
		cmp al, 97; //compare upper
		jl CONTINUE;

		cmp al, 122; //compare lower
		jg CONTINUE;

		//start on lowercase
		and al, 95;
		mov byte ptr[edx + esi], al;

	CONTINUE:
		//Not a letter
		inc esi; //increase to next
		jmp BEGIN_LOOP;
	END_LOOP:
		mov char_array, edx;


			// END YOUR CODE HERE
	}
}

/***********************************************************************************************

    PART 5: Fibonacci

	Implement a recursive function in assembly which returns the Nth number in the fibbonaci
	series (1, 1, 2, 3, 5, 8, ...).

************************************************************************************************/

__declspec(naked) 
unsigned int fibonacci (unsigned int n) {

// C code to be converted to x86 assembly
/*
	if (n < 2) 
		return 1;
	else 
		return fibonacci(n - 1) + fibonacci(n - 2);
*/

	__asm{
			// BEGIN YOUR CODE HERE
		//push ebp onto stack
		push ebp;
		//set esp pointed to ebp
		mov ebp, esp;

		//push to stack
		push edi;
		push esi;
		push ebx;

		//push value onto ebx
		mov ebx, [ebp + 8];
		cmp ebx, 2; //if n < 2 check
		jae RECURSIVE_LOOP; //if not less than 2

		mov eax, 1; //return 1 -> jump to end
		jmp END;
	RECURSIVE_LOOP:
		//push on to stack to store value 
		push eax;
		push ecx;
		push edx;

		//move ebx to point ecx
		mov ecx, ebx;
		dec ecx;//-1

		//push new ecx
		push ecx;

		call fibonacci; //first fib (the value is lower by 1)

		add esp, 4; //reserve space

		mov esi, eax; //take returned value from eax via fib 1

		mov ecx, ebx; //reset ecx with ebx
		sub ecx, 2;//-2

		push ecx;

		call fibonacci; //second fib (value is lower by 2)

		add esp, 4;//reserve space

		mov edi, eax;//takes value from eax via fib 2
		//pop stack
		pop edx;
		pop ecx;
		pop eax;

		//mov esi to eax, then add eax with edi
		mov eax, esi; //esi contains the returned fib value
		add eax, edi; //the returned fib value + returned fib value 2

	END:
		//remove from stack
		pop ebx;
		pop esi;
		pop edi;

		mov esp, ebp; //remove pointer
		pop ebp;


			// END YOUR CODE HERE
			ret
	}
}