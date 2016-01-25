#include <stdio.h>

int main()
{
	int n;
	scanf("%d", &n);

	float cur_pos[2];
	scanf("%f %f", &cur_pos[0], &cur_pos[1]);
	float bidoni[n][2];
	int i;

	for (i = 0; i < n; i++)
	{
		scanf("%f %f", &bidoni[i][0], &bidoni[i][1]);
	}

	float weight = 1.00;
	float delta_x;
	float delta_y;
	for (i = 0; i < n; i++)
	{
		delta_x = bidoni[i][0] - cur_pos[0];
		delta_y = bidoni[i][1] - cur_pos[1];

		// slide = current delta / weight
		delta_x = delta_x + delta_x/weight;
		delta_y = delta_y + delta_y/weight;

		printf("%0.2f %0.2f\n", delta_x, delta_y);

		cur_pos[0] = cur_pos[0] + delta_x;
		cur_pos[1] = cur_pos[1] + delta_y;
		weight = weight * 2;
	}
	
	printf("%0.2f %0.2f\n", cur_pos[0], cur_pos[1]);	
	return 0;	
}