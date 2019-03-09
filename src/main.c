#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define RCDIR ".genctemplate"

enum TemplateType
{
  TEMPLATE_TYPE_C,  // "c"
  TEMPLATE_TYPE_MAKEFILE // "makefile"
};

// list of function signatures
static bool wrapper_write(const char* filepath, enum TemplateType tt);
static int get_file_total_size(FILE* fp);
static char* read_template_file(enum TemplateType tt, long* rst_size);
static bool write_c_template(const char* filepath);
static bool write_makefile_template(const char* filepath);
static void set_user_homedir(const char** ptr);

// to be filled for user's home directory
static const char* user_homedir = NULL;

///
/// Set user's home directory to input pointer
///
void set_user_homedir(const char** ptr)
{
  // get home directory via HOME environment variable
  const char* homedir = getenv("HOME");
  *ptr = homedir;
}

///
/// Wrapper to execute writing into template file base on input template type,
/// and output file.
///
bool wrapper_write(const char* filepath, enum TemplateType tt)
{
  if (tt == TEMPLATE_TYPE_C)
  {
    return write_c_template(filepath);
  }
  else if (tt == TEMPLATE_TYPE_MAKEFILE)
  {
    return write_makefile_template(filepath);
  }
  else
  {
    fprintf(stderr, "Error unrecognize template type\n");
    return false;
  }
}

///
/// Get total file size from the input file pointer
///
/// \param fp file pointer
/// \return total file size in bytes
///
int get_file_total_size(FILE* fp)
{
  // set position of file back later when we done
  long old_pos = ftell(fp);
  
  // get the total size of file
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);

  // set the old position back
  fseek(fp, old_pos, SEEK_SET);

  return size;
}

char* read_template_file(enum TemplateType tt, long* rst_size)
{
  // form template file path to read from
  char template_filepath[512];
  if (tt == TEMPLATE_TYPE_C)
  {
    if (snprintf(template_filepath, 256, "%s/%s/templates/c.template", user_homedir, RCDIR) >= 256)
    {
      fprintf(stderr, "Error forming user's home directory path. The path might be too long.\n");
      return NULL;
    }
  }
  else
  {
    if (snprintf(template_filepath, 256, "%s/%s/templates/Makefile.template", user_homedir, RCDIR) >= 256)
    {
      fprintf(stderr, "Error forming user's home directory path. The path might be too long.\n");
      return NULL;
    }
  }

  // read Makefile template from file
  FILE* fr = fopen(template_filepath, "r");
  if (fr == NULL)
  {
    fprintf(stderr, "Cannot open Makefile template at %s\n", template_filepath);
    return NULL;
  }

  // get total bytes of template file
  long template_file_size = get_file_total_size(fr);

  // dynamically allocate memory to hold template content
  char* rmem = calloc(1, sizeof(char) * (template_file_size+1));

  if (fread(rmem, template_file_size-1, 1, fr) != 1)
  {
    fprintf(stderr, "Error reading template file %s\n", template_filepath);

    // close template file
    fclose(fr);
    fr = NULL;

    return NULL;
  }
  // close template file
  fclose(fr);
  fr = NULL;

  // set result read size
  if (rst_size != NULL)
    *rst_size = template_file_size;

  // return read content of file
  return rmem;
}

bool write_c_template(const char* filepath)
{
  FILE* fo = fopen(filepath, "w");
  if (fo == NULL)
  {
    fprintf(stderr, "Cannot open file for write at %s\n", filepath);
    return false;
  }

  // read template file
  long read_size = 0;
  char* rmem = read_template_file(TEMPLATE_TYPE_C, &read_size);
  if (rmem == NULL)
    return false;

  // write content of template file into output file
  if (fwrite(rmem, read_size, 1, fo) != 1)
  {
    fprintf(stderr, "Error writing output file\n");

    // free read mem
    free(rmem);
    // close output file
    fclose(fo);

    return false;
  }

  free(rmem);
  fclose(fo);
  return true;
}

bool write_makefile_template(const char* filepath)
{
  // open output file for writing
  FILE* fo = fopen(filepath, "w");
  if (fo == NULL)
  {
    fprintf(stderr, "Cannot open file for write at %s\n", filepath);
    return false;
  }
  
  // read template file
  long read_size = 0;
  char* rmem = read_template_file(TEMPLATE_TYPE_MAKEFILE, &read_size);
  if (rmem == NULL)
    return false;

  // write content of template file into output file
  if (fwrite(rmem, read_size, 1, fo) != 1)
  {
    fprintf(stderr, "Error writing output file\n");

    // free read mem
    free(rmem);
    // close output file
    fclose(fo);

    return false;
  }

  // free read mem
  free(rmem);
  // close output file
  fclose(fo);

  // SUCCESS
  return true;
}

int main(int argc, char* argv[])
{
  // if not enough parameters then print out help
  if (argc < 2)
  {
    fprintf(stderr, "Usage: genctemplate [-t <c,makefile>] /your/path/filename\n");
    return -1;
  }
  // convenient for generating c template without needing to specify -t
  if (argc == 2)
  {
    if (wrapper_write(argv[1], TEMPLATE_TYPE_C))
    {
      fprintf(stdout, "Template file ready at %s\n", argv[1]);
    }
  }
  // now for other types
  else if (argc > 2)
  {
    // get user's home directory
    set_user_homedir(&user_homedir);

    // check if user specifies all required parameters
    if (argc < 4)
    {
      fprintf(stderr, "Usage genctemplate -t <c,makefile> /your/path/outputfile\n");
      return -1;
    }
    else
    {
      // set default to template type c
      // it can be a case that user explicitly specifies the type of template to generate
      enum TemplateType tt = TEMPLATE_TYPE_C;
      int type_param_index = -1;

      for (int i=1; i<argc; ++i)
      {
        if (strncmp(argv[i], "-t", 2) == 0)
        {
          // check boundary
          if (i+1 >= argc)
          {
            fprintf(stderr, "Error not template type (for -t) exists\n");
            return -1;
          }

          // save type index
          type_param_index = i;

          // then the next parameter must be the type
          // TODO: support case insensitive?
          if (strncmp(argv[i+1], "c", 1) == 0)
          {
            tt = TEMPLATE_TYPE_C;
          }
          else if (strncmp(argv[i+1], "makefile", 8) == 0)
          {
            tt = TEMPLATE_TYPE_MAKEFILE;
          }
          // ERROR case
          else
          {
            fprintf(stderr, "Error unrecognized template type. Possible is c, makefile\n");
            return -1;
          }
        }
      }     

      // get output file
      if (type_param_index == 1)
      {
        if (wrapper_write(argv[argc-1], tt))
        {
          fprintf(stdout, "Template file ready at %s\n", argv[argc-1]);
        }
      }
      else if (type_param_index == argc-2)
      {
        if (wrapper_write(argv[1], tt))
        {
          fprintf(stdout, "Template file ready at %s\n", argv[1]);
        }
      }
      else
      {
        fprintf(stderr, "Error, please specify output file name\n");
        return -1;
      }
    }
  }

  return 0;
}
