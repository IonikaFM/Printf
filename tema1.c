#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static int write_stdout(const char *token, int lenght)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, lenght - bytes_written);
		if(rc < 0)
			return rc;
		bytes_written += rc;
	} while (bytes_written < lenght);

	return bytes_written;
}

static int lenght(unsigned int number)
{
	int l = 0;
	while(number)
	{
		l ++;
		number /= 10;
	}
	return l;
} //functie ce returneaza lungimea unui numar

int iocla_printf(const char *format, ...)
{
	va_list args;

	int i, j, k = 0, aux, nr;
	unsigned int arg;

	char *s = (char *)malloc(20000 * sizeof(char));

	char *final = (char *)malloc(20000 * sizeof(char));
	//final este stringul in care salvez ce vreau sa afiseze functia iocla_printf

	va_start(args, format);


	for (i = 0; i <= strlen(format); i++)
	{
		if(format[i] == '%')
		{
			switch (format[i+1])
			{
			case  'd':
				j = va_arg(args, int);

				if(j == 0)
					final[k++] = '0';
					//daca este 0 il adauga direct in string si iese din switch
				else
				{
					if(j < 0)
						final[k++] = '-';
						//daca este negativ adauga prima oara semnul "-" in string

					nr = lenght(abs(j));

					for(aux = nr - 1; aux >= 0; aux --)
					//scrie in final, de la dreapta la stanga, cifrele numarului 
					{
						final[k + aux] = abs(j % 10)+ '0';
						//ia modulul fiecare cifra din j, o transforma in char si o adauga pe pozitia corecta din stringul final
						j /= 10;
					} 
					k += nr;
					//creste marimea stringului final
				}
				i++;
				//sarim peste caracterul de dupa "%"

				break;
			
			case 'u':
				arg = va_arg(args, unsigned int);

				nr = lenght(arg);

				for(aux = nr - 1; aux >= 0; aux --)
				{
					final[k + aux] = arg % 10 + '0';
					//transforma fiecare cifra din arg in char si o adauga pe pozitia corecta din stringul final
					arg /= 10;
				}

				k += nr;
				i ++;
				break;

			case 'c':
				final[k++] = va_arg(args, int);
				//adauga caracterul agument in sirul final
				i++;
				break;

			case 'x':
				arg = va_arg(args, unsigned int);

				nr = 0;
				strcpy(s, "");

				while(arg)
				{
					aux = arg % 16;
					if(aux < 10)
						aux += 48;
					else
						aux += 55;
					s[nr++] = aux;
					arg /= 16;
				}//transforma un numar din zecimal in hexazecimal si il salveaza in stringul s

				for(aux = nr - 1; aux >= 0; aux --)
				{
					if(s[aux] >= 65 && s[aux] <= 90)
						s[aux] += 32;
						//daca avem caractere intre 'A' si 'Z' le facem downcase in 'a' - 'z'
					final[k + nr - 1 - aux] = s[aux] ;
					//adauga fiecare caracter din s pe pozitia corecta din stringul final
				}
				k += nr;
				i ++;
				break;
			
			case 's':
				strcpy(s,va_arg(args, char*));

				strcat(final, s);
				k += strlen(s);
				//concatenez sirul s la sirul final si incrementez lungimea sirului final
				i++;
				break;

			case '%':
				final[k++] = '%';
				i++;
				break;
			}
		}
		else
			final[k++] = format[i];
	}
	va_end(args);
	free(s);
	write_stdout(final, strlen(final));
	//scriem sirul final folosing functia write_stdout
	return strlen(final);
	free(final);
}