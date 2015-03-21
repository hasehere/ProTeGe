#include "ImageLib.cpp"
#include "cellular_noise_lib.cpp"

float s_curve(float x)
{
    return powf(x,5) * 6 - powf(x,4) * 15 + powf(x, 3) * 10;
    //return x;
}

class perlin_vector
{
public:
    float x;
    float y;

    void gen()
    {
        if (rand() > RAND_MAX / 2)
        {
            y = (float)rand() * 2.0 / (float)RAND_MAX - 1;
            x = sqrt(1 - y * y);
            if (rand() > RAND_MAX / 2) x = -x;
        }
        else
        {
            x = (float)rand() * 2.0 / (float)RAND_MAX - 1;
            y = sqrt(1 - x * x);
            if (rand() > RAND_MAX / 2) y = -y;
        }
    }
};

class vector_field
{
public:
    perlin_vector ** vectors;

    int size;

    vector_field(int s)
    {
        size = s;

        vectors = new perlin_vector * [size];

        for(int i = 0; i < size; i++)
        {
            vectors[i] = new perlin_vector[size];
        }

        for (int x = 0; x < size; x++)
        {
            for (int y = 0; y < size; y++)
            {
                vectors[x][y].gen();
            }
        }
    }

    ~vector_field()
    {
        for(int i = 0; i < size; i++)
        {
            delete vectors[i];
        }

        delete[] vectors;
    }
};

greyscale * perlin_noise(int detail_level, vector_field * field)
{
    int detail_coef = 2048 / detail_level;

    float detail_coef_sqr = sqrt(2) / (float)detail_coef;

    greyscale * result = new greyscale(2048);

    float ** precomputed_distances = new float*[detail_coef];

    for (int i = 0; i < detail_coef + 1; i++)
    {
        precomputed_distances[i] = new float[detail_coef];
    }

    for (int px = 0; px <= detail_coef; px ++)
    {
        for (int py = 0; py <= detail_coef; py ++)
        {
            precomputed_distances[px][py] = s_curve((1 - (float)px / detail_coef) * (1 - (float)py / detail_coef) );
        }
    }

    // do it

    float point_colors[4];

    int vector_field_x;
    int vector_field_y;
    int vector_field_x_real;
    int vector_field_y_real;
    float x_mult;
    float y_mult;
    int x_dist;
    int y_dist;
    float distance;
    float distance_sum;

    for(int y = 0; y < 2048; y++)
    {
        for(int x = 0; x < 2048; x++)
        {
            distance_sum = 0;

            //make each pixel

            vector_field_x = x * detail_level / 2048;
            vector_field_y = y * detail_level / 2048;

            x_dist = x - vector_field_x * detail_coef;
            y_dist = y - vector_field_y * detail_coef;

            x_mult = (float)x_dist * field->vectors[vector_field_x][vector_field_y].x * detail_coef_sqr;
            y_mult = (float)y_dist * field->vectors[vector_field_x][vector_field_y].y * detail_coef_sqr;

            distance = precomputed_distances[x_dist][y_dist];

            distance_sum += distance;

            point_colors[0] = (x_mult + y_mult) / 2.0 * distance;

            // right

            vector_field_x = x * detail_level / 2048 + 1;
            vector_field_y = y * detail_level / 2048;

            x_dist = x - vector_field_x * detail_coef;
            y_dist = y - vector_field_y * detail_coef;

            vector_field_x_real = vector_field_x % detail_level;

            x_mult = (float)x_dist * field->vectors[vector_field_x_real][vector_field_y].x * detail_coef_sqr;
            y_mult = (float)y_dist * field->vectors[vector_field_x_real][vector_field_y].y * detail_coef_sqr;

            distance = precomputed_distances[0 - x_dist][y_dist];

            distance_sum += distance;

            point_colors[1] = (x_mult + y_mult) / 2.0 * distance;

            //top

            vector_field_x = x * detail_level / 2048;
            vector_field_y = y * detail_level / 2048 + 1;

            x_dist = x - vector_field_x * detail_coef;
            y_dist = y - vector_field_y * detail_coef;

            vector_field_y_real = vector_field_y % detail_level;

            x_mult = (float)x_dist * field->vectors[vector_field_x][vector_field_y_real].x * detail_coef_sqr;
            y_mult = (float)y_dist * field->vectors[vector_field_x][vector_field_y_real].y * detail_coef_sqr;

            distance = precomputed_distances[x_dist][0 - y_dist];
            distance_sum += distance;

            point_colors[2] = (x_mult + y_mult) / 2.0 * distance;

            //diagonal

            vector_field_x = x * detail_level / 2048 + 1;
            vector_field_y = y * detail_level / 2048 + 1;

            x_dist = x - vector_field_x * detail_coef;
            y_dist = y - vector_field_y * detail_coef;

            vector_field_y_real = vector_field_y % detail_level;
            vector_field_x_real = vector_field_x % detail_level;

            x_mult = (float)x_dist * field->vectors[vector_field_x_real][vector_field_y_real].x * detail_coef_sqr;
            y_mult = (float)y_dist * field->vectors[vector_field_x_real][vector_field_y_real].y * detail_coef_sqr;

            distance = precomputed_distances[0 - x_dist][0 - y_dist];

            distance_sum += distance;

            point_colors[3] = (x_mult + y_mult) / 2.0 * distance;

            //point_colors[3] = distance;

            result->data[x][y] = (point_colors[0] + point_colors[1] + point_colors[2] + point_colors[3])  / distance_sum;
            // -1 to 1
        }

    }

    for (int i = 0; i < detail_coef + 1; i++)
    {
        delete[] precomputed_distances[i];
    }

    delete[] precomputed_distances;

    return result;
}

greyscale * fractal_perlin_noise(int * distribution)
{
    vector_field * field = new vector_field(1024);

    int detail_level = 9;

    greyscale ** bases = new greyscale*[detail_level];

    for (int i = 0; i < detail_level; i++)
    {
        if(distribution[i] == 0)
        {
            bases[i] = new greyscale(2048);
        }
        else
        {
            bases[i] = perlin_noise(powf(2,i + 1), field);
        }

    }

    greyscale * result = new greyscale(2048);

    for (int x = 0; x < 2048; x++)
    {
        for (int y = 0; y < 2048; y++)
        {
            int distribution_sum = 0;
            for (int i = 0; i < detail_level; i++)
            {
                if (distribution[i] != 0) result->data[x][y] += bases[i]->data[x][y] * distribution[i];
                distribution_sum += distribution[i];
            }
            result->data[x][y] = result->data[x][y] / distribution_sum;
        }
    }

    delete field;

    return result;
}

greyscale * fractal_perlin_noise(float distribution_coef)
{

    float f_distribution[10];
    int distribution[10];

    f_distribution[9] = 2;

    for (int i = 8; i >= 0; i--)
    {
        f_distribution[i] = f_distribution[i + 1] * distribution_coef;
        distribution[i + 1] = int(f_distribution[i + 1]);
        //wxMessageBox(wxString::Format(wxT("%i"), distribution[i +  1]));
    }
    distribution[0] = int(f_distribution[0]);

    return fractal_perlin_noise(distribution);
}

/*
tesing stuff
*/

greyscale * table(int detail_level)
{
    greyscale * result = new greyscale(2048);

    float detail_coef = 2048 / detail_level;

    int vector_field_x;
    int vector_field_y;

    for(int y = 0; y < 2048; y++)
    {
        for(int x = 0; x < 2048; x++)
        {
            vector_field_x = x * detail_level / 2048;
            vector_field_y = y * detail_level / 2048;

            result->data[x][y] = 1;

            if (x == vector_field_x * detail_coef or x == vector_field_x * detail_coef + 1 or x == vector_field_x * detail_coef - 1 or y == vector_field_y * detail_coef or y == vector_field_y * detail_coef + 1 or y == vector_field_y * detail_coef - 1)
            {
                    result->data[x][y] = 0;
            }
        }

    }

    return result;
}

greyscale * dist(int detail_level)
{
    greyscale * result = new greyscale(2048);

    float detail_coef = 2048 / detail_level;

    float ** precomputed_distances = new float*[int(detail_coef)];

    for (int i = 0; i < detail_coef + 1; i++)
    {
        precomputed_distances[i] = new float[int(detail_coef + 1)];
    }

    for (int px = 0; px < (int)detail_coef + 1; px ++)
    {
        for (int py = 0; py < (int)detail_coef + 1; py ++)
        {
            precomputed_distances[px][py] = (1 - (float)px / detail_coef) * (1 - (float)py / detail_coef);
        }
    }

    // do it

    float point_colors[4];

    int vector_field_x;
    int vector_field_y;
    int x_dist;
    int y_dist;
    float distance;

    for(int y = 0; y < 2048; y++)
    {
        for(int x = 0; x < 2048; x++)
        {

            vector_field_x = x * detail_level / 2048;
            vector_field_y = y * detail_level / 2048;

            x_dist = x - vector_field_x * detail_coef;
            y_dist = y - vector_field_y * detail_coef;

            distance = precomputed_distances[x_dist][y_dist];

            result->data[x][y] = distance * 2 - 1;
        }

    }

    for (int i = 0; i < detail_coef + 1; i++)
    {
        delete[] precomputed_distances[i];
    }

    delete[] precomputed_distances;

    return result;
}
