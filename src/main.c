#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum TemplateType
{
  TEMPLATE_TYPE_C,  // "c"
  TEMPLATE_TYPE_MAKEFILE // "makefile"
};

// list of function signatures
static bool wrapper_write(const char* filepath, enum TemplateType tt);
static int get_file_total_size(FILE* fp);
static bool write_c_template(const char* filepath);
static bool write_makefile_template(const char* filepath);

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

bool write_c_template(const char* filepath)
{
  FILE* file = fopen(filepath, "w");
  if (file == NULL)
  {
    fprintf(stderr, "Cannot open file for write at %s\n", filepath);
    return false;
  }

  fprintf(file, "#include <stdio.h>\n\nint main (int argc, char* argv[])\n{\n\n\treturn 0;\n}\n");

  fclose(file);
  return true;
}

bool write_makefile_template(const char* filepath)
{
  // read Makefile template from file
  FILE* fr = fopen("templates/Makefile.template", "r");
  if (fr == NULL)
  {
    fprintf(stderr, "Cannot open Makefile tempalte at templates/Makefile.template\n");
    return false;
  }

  // get total bytes of template file
  long template_file_size = get_file_total_size(fr);
  // allocate memory to hold template content
  char rmem[template_file_size+1];
  if (fread(rmem, template_file_size, 1, fr) != 1)
  {
    fprintf(stderr, "Error reading template file\n");

    // close template file
    fclose(fr);
    fr = NULL;

    return false;
  }
  // close template file
  fclose(fr);
  fr = NULL;

  // open output file for writing
  FILE* fo = fopen(filepath, "w");
  if (fo == NULL)
  {
    fprintf(stderr, "Cannot open file for write at %s\n", filepath);
    return false;
  }

  // write content of template file into output file
  if (fwrite(rmem, sizeof(rmem) - 1, 1, fo) != 1)
  {
    fprintf(stderr, "Error wriring output file\n");

    // close output file
    fclose(fo);
    fo = NULL;

    return false;
  }

  // close output file
  fclose(fo);
  fo = NULL;

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
      fprintf(stdout, "Template file aready at %s\n", argv[1]);
    }
  }
  // now for other types
  else if (argc > 2)
  {
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
          fprintf(stdout, "Template file aready at %s\n", argv[argc-1]);
        }
      }
      else if (type_param_index == argc-2)
      {
        if (wrapper_write(argv[1], tt))
        {
          fprintf(stdout, "Template file aready at %s\n", argv[1]);
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
