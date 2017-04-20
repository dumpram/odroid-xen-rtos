/**
 * @ brief:
 *
 * Tests for helper functions.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */

void dump_register(char *out, char *reg_name, unsigned value);

static char buffer[100];

int main()
{
    
    dump_register(buffer, "TEST", 0);
    printf("BUFFER: %s\n", buffer);

    dump_register(buffer, "TEST1", 12345);
    printf("BUFFER1: %s\n", buffer);

    return 0;
}
