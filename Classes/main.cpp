#include <complex>
#include <cstdio>
#include <time.h>
#include <omp.h>

typedef std::complex<double> complex;

int MandelbrotCalculate(complex c, int maxiter)
{
	// iterates z = z + c until |z| >= 2 or maxiter is reached,
	// returns the number of iterations.
	complex z = c;
	int n=0;
	for(; n<maxiter; ++n)
	{
		if( std::abs(z) >= 2.0) break;
		z = z*z + c;
	}
	return n;
}

int main()
{
	clock_t tStart = clock();

	

	const int width = 160, height = 100, num_pixels = width*height;
	
	const complex center(-.7, 0), span(2.7, -(4/3.0)*2.7*height/width);
	const complex begin = center-span/2.0;//, end = center+span/2.0;
	const int maxiter = 1000000;

	#pragma omp parallel for ordered schedule(dynamic) num_threads(32)
	for(int pix=0; pix<num_pixels; ++pix)
	{
		const int x = pix%width, y = pix/width;
		
		complex c = begin + complex(x * span.real() / (width +1.0),
									y * span.imag() / (height+1.0));
		
		int n = MandelbrotCalculate(c, maxiter);
		if(n == maxiter) n = 0;
		
		#pragma omp ordered
		{
			char c = ' ';
			if(n > 0)
			{
				static const char charset[] = ".,c8M@jawrpogOQEPGJ";
				c = charset[n % (sizeof(charset)-1)];
			}
			std::putchar(c);
			if(x+1 == width) std::puts("|");
		}
	}

	printf("Time taken: %.5fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	int a = 0;

	scanf("%i", &a);
}