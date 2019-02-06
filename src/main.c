#include <stdio.h>

int write_normal_template(const char* filepath)
{
  FILE* file = fopen(filepath, "w");
  if (file == NULL)
  {
    fprintf(stderr, "Cannot open file for write at %s\n", filepath);
    return -1;
  }

  fprintf(file, "#include <stdio.h>\n\nint main (int argc, char* argv[])\n{\n\n\treturn 0;\n}\n");

  fclose(file);
  return 0;
}

int main(int argc, char* argv[])
{
  // receive destination file path
  if (argc != 2)
  {
    fprintf(stderr, "Usage: genctemplate /your/path/test.c\n");
    return -1;
  }

  if (write_normal_template(argv[1]) == 0)
  {
    fprintf(stdout, "Template generated\n");
  }
  return 0;
}
