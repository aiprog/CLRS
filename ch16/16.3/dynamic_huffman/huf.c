#include <sys/stat.h>

#include "dy_huf_tree.h"

Node *ite;
FILE *rp, *wp;
unsigned char *input_buf, *output_buf;
unsigned long input_ptr, output_ptr;
unsigned long file_size;

void get_file_size (char *file)
{
  struct stat file_stat;

  if (stat (file, &file_stat) != 0)
  {
    perror ("Error");
    exit (1);
  }
  file_size = file_stat.st_size;
}

void decompress (char *input, char *output)
{
  int quo, rem, bit;

  initialize_tree ();

  get_file_size (input);
  input_buf = malloc (file_size);
  output_buf = malloc (file_size);

  if ((rp = fopen (input, "r")) == NULL || (wp = fopen (output, "w")) == NULL)
  {
    perror ("Error");
    exit (1);
  }
  fread (input_buf, 1, file_size, rp);

  ite = root;
  input_ptr = -1;
  output_ptr = 0;
  while (1)
  {
    ++input_ptr;

    quo = (input_ptr + 1) / 8;
    rem = (input_ptr + 1) % 8;
    if (rem == 0)
      bit = input_buf[quo - 1] & 0x01;
    else
      bit = input_buf[quo] >> (8 -  rem) & 0x01;

    if (bit == 0)
      ite = ite->left;
    else
      ite = ite->right;

    if (ite->ch == -1)
      continue;
    else if (ite->ch > -1 && ite->ch < 127)
    {
      if (output_ptr == file_size)
      {
	fwrite (output_buf, 1, file_size, wp);
	output_ptr = 0;
      }

      output_buf[output_ptr++] = ite->ch;
      adjust_tree (table[ite->ch]);
      ite = root;
    }
    else
    {
      fwrite (output_buf, 1, output_ptr, wp);
      break;
    }
  }

  fclose (rp);
  fclose (wp);
}

void compress (char *input, char *output)
{
  char bit[CHA_SET];
  int bit_ptr, byte_ptr;
  int output_byte;
 
  initialize_tree ();

  get_file_size (input);
  input_buf = malloc (file_size + 1);
  output_buf = malloc (file_size + 1);

  if ((rp = fopen (input, "r")) == NULL || (wp = fopen (output, "w")) == NULL)
  {
    perror ("Error");
    exit (1);
  }
  fread (input_buf, 1, file_size, rp);
  input_buf[file_size] = 127;

  output_byte = 0;
  bit_ptr = CHA_SET;
  input_ptr = output_ptr = byte_ptr = 0;
  while (1)
  {
    while (byte_ptr != 8 && bit_ptr != CHA_SET)
    {
      output_byte <<= 1;
      if (bit[bit_ptr] == 1)
	output_byte += 1;

      ++byte_ptr;
      ++bit_ptr;
    }

    if (byte_ptr == 8)
    {
      if (output_ptr == file_size - 1)
      {
	printf ("Can not be compressed!\n");
	exit (1);
      }

      output_buf[output_ptr++] = output_byte;
      output_byte = 0;
      byte_ptr = 0;
    }
    
    if (bit_ptr == CHA_SET)
    {
      if (input_ptr <= file_size)
      {
        for (ite = table[input_buf[input_ptr]]; ite != root; ite = ite->parent)
        {
	  if (ite == ite->parent->left)
	    bit[--bit_ptr] = 0;
	  else
	    bit[--bit_ptr] = 1;
        }

        adjust_tree (table[input_buf[input_ptr]]);
	++input_ptr;
      }
      else 
      {
	if (byte_ptr != 0)
	{
	  if (output_ptr == file_size - 1)
	  {
	    printf ("Can not be compressed!\n");
	    exit (1);
	  }

	  while (byte_ptr++ != 8)
	    output_byte <<= 1;
	  output_buf[output_ptr++] = output_byte;
	}

	break;
      }
    }
  }

  fwrite (output_buf, 1, output_ptr, wp);

  fclose (rp);
  fclose (wp);
}

int main (int argc, char **argv)
{
  if (argc != 4)
  {
    printf ("Usage: huf -(c|d) FILENAME FILENAME\n");
    return 1;
  }

  if (argv[1][1] == 'c')
    compress (argv[2], argv[3]);
  else if (argv[1][1] == 'd')
    decompress (argv[2], argv[3]);
  else
  {
    printf ("Unrecognized option %c\n", argv[1][1]);
    return 0;
  }

  return 0;
}
