class cell
{
public:
    int x;
    int y;
};

class cell_field
{
public:
    cell ** data;
    int size;

    cell_field(int s)
    {
        size = s;
        data = new cell*[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = new cell[size];
        }

        for (int x = 0; x < size; x++)
        {

        }
    }
    ~cell_field()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] data[i];
        }
        delete data;
    }

    float find_distance(int x, int y, int offset_x, int offset_y, int pixel_x, int pixel_y)
    {
        return 2;
    }

    float find_min_distance(int x, int y, int pixel_x, int pixel_y, float ** distance_table)
    {
        int offsets[9][2] = {{0,1},{1,0},{1,1},{-1,0},{0,-1},{-1,-1},{-1,1},{1,-1}, {0,0}};

        float min_distance = distance_table[size][size];

        for (int i = 0; i < 9; i++)
        {
            float cur_distance = find_distance(x,y, offsets[i][0], offsets[i][1], pixel_x, pixel_y);
            if (cur_distance <= min_distance)
            {
                min_distance = cur_distance;
            }
        }

        return min_distance;
    }
};

/*greyscale * cellular_noise(int detail_level)
{

}
*/
