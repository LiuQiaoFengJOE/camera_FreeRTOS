/**@file 	build.s
 * @brief 	This file is just used to rebuild app everytime when you update the library.
 * Do you know why it can be?
 * Not like .S file, the .s file cannot be preprocess by gcc.
 * So .d file cannot be generated and cause `make' to rebuild .s file everytime.
 * @author 	appo 
 * @date 	2017-08-03
 */

/* So it cannot #include <or1k-asm.h>
 * And .nodelay should be hand-writed
 */
.nodelay

/*


		*	*						*	*
				*				*		
	*				*		*				*

	*					*					*

		*								*

			*						*
	
				*				*
					*		*
						*
*/