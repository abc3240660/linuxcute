#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int value[5] = {0};
	int center_val = 0;

	printf("Please input center value:");
	scanf("%d", &center_val);

	printf("center_val = %d\n", center_val);

	if (500 == center_val) {
		value[0] = 0;
		value[1] = 0;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val >= 475) && (center_val < 500)) {
		value[0] = 0;
		value[1] = 500;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val > 450) && (center_val < 475)) {
		value[0] = 500;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val >= 225) && (center_val <= 450)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val > 200) && (center_val < 225)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = 175;
	} else if ((center_val > 175) && (center_val <= 200)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = 175;
		value[4] = 0;
	} else if (175 == center_val) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = 0; 
		value[4] = 0;
	}

	for (int i=0; i<5; i++) {
		if (value[i] != 0) {
			printf("value[%d] = %d\n", i, value[i]);
		} else {
			printf("value[%d] = XXX\n", i);
		}
	}

	return 0;
}
