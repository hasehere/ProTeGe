#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include <math.h>

class pixel
{
public:
    int R;
    int G;
    int B;

    pixel()
    {
        R = 0;
        G = 0;
        B = 0;
    }

    ~pixel() {}
};

class image
{
public:
    pixel ** data;
    int size;

    image(int x)
    {
        size = x;

        data = new pixel * [size];

        for (int i = 0; i < size; i++)
        {
            data[i] = new pixel[size];
        }
    }

    void set_image(wxStaticBitmap * dest, int scale)
    {
        wxImage * temp_wximage = new wxImage(size, size, true);

         for (int x = 0; x < size; x++)
         {
             for (int y = 0; y < size; y ++)
             {
                 pixel cur = data[x][y];
                 temp_wximage->SetRGB(x, y, cur.R, cur.G, cur.B);
             }
         }


         //temp_wximage->Rescale(scale, scale, wxIMAGE_QUALITY_HIGH);

         wxBitmap * temp_wxbitmap = new wxBitmap(*temp_wximage);

         dest->SetBitmap(*temp_wxbitmap);

         delete temp_wximage;
         delete temp_wxbitmap;
    }

    ~image()
    {
        for (int i = 0; i < size; i++) delete[] data[i];
        delete[] data;
    }
};

class greyscale
{
private:
    float get_hor_neighbour(int x, int y, int offset)
    {
        if (offset == 0) return data[x][y];
        if (offset > 0)
        {
            int new_x = (x + offset) % size;
            return data[new_x][y];
        }
        //offset is < 0
        int new_x = x + offset;
        while(new_x < 0) new_x = size + new_x; //cycle from other side
        return data[new_x][y];
    }

    float get_vert_neighbour(int x, int y, int offset)
    {
        if (offset == 0) return data[x][y];
        if (offset > 0)
        {
            int new_y = (y + offset) % size;
            return data[x][new_y];
        }
        //offset is < 0
        int new_y = y + offset;
        while(new_y < 0) new_y = size + new_y; //cycle from other side
        return data[x][new_y];
    }
public:
    int size;
    float ** data;

    greyscale()
    {
        size = 2048;

        data = new float * [size];

        for (int i = 0; i < size; i++)
        {
            data[i] = new float[size];
        }
    }

    greyscale(int x)
    {
        size = x;

        data = new float * [size];

        for (int i = 0; i < size; i++)
        {
            data[i] = new float[size];
        }
    }

    static float curve(float x)
    {
        return pow(x,5) * 6 - pow(x,4) * 15 + pow(x, 3) * 10;
        //return x;
    }

    void normalize()
    {
        for(int x = 0; x < 2048; x++)
        {
            for(int y = 0; y < 2048; y++)
            {
                data[x][y] = (data[x][y] + 1.0) * 255.0 / 2.0;
            }
        }
    }

    void normalize_pre_fog()
    {
        for(int x = 0; x < 2048; x++)
        {
            for(int y = 0; y < 2048; y++)
            {
                data[x][y] = (curve((data[x][y]  + 1.0) / 2) * 2) - 1;
            }
        }
    }

    void normalize_pre_fire()
    {
        for(int x = 0; x < 2048; x++)
        {
            for(int y = 0; y < 2048; y++)
            {
                float value = data[x][y];
                if (value < 0) value = 0 - value;
                data[x][y] = 1.0 - value * 2.0;
            }
        }
    }

    void normalize_pre_marble(int scale, float noise_coef)
    {
        for(int x = 0; x < 2048; x++)
        {
            for(int y = 0; y < 2048; y++)
            {
                float value = data[x][y];
                value = sin(float(x) * M_PI / float(scale) + value * noise_coef * M_PI);
                data[x][y] = value;
            }
        }
    }

    void normalize_pre_wood(int scale, float noise_coef)
    {
        for(int x = 0; x < 2048; x++)
        {
            for(int y = 0; y < 2048; y++)
            {
                float value = data[x][y];
                value = sin(sqrt(pow(x - 1024,2) + pow(y - 1024,2)) * M_PI /  float(scale) + value * noise_coef * M_PI);
                data[x][y] = value;
            }
        }
    }

    void maximize_contrast()
    {
        float max = 0;
        float min = 255;

        for(int x = 0; x < 2048; x++)
        {
            for (int y = 0; y < 2048; y++)
            {
                if (data[x][y] < min) min = data[x][y];
                if (data[x][y] > max) max = data[x][y];
            }
        }

        max = max - min;

        for(int x = 0; x < 2048; x++)
        {
            for (int y = 0; y < 2048; y++)
            {
                data[x][y] = (data[x][y] - min) * 255.0 / max;
            }
        }

    }

    greyscale * upscale()
    {
        greyscale * scaled = new greyscale(size * 2);

        //transfer the data

        return scaled;
    }

    void set_image(wxStaticBitmap * dest, int scale)
    {
        wxImage * temp_wximage = new wxImage(size, size, true);

         for (int x = 0; x < size; x++)
         {
             for (int y = 0; y < size; y ++)
             {
                 temp_wximage->SetRGB(x, y, (int)data[x][y], (int)data[x][y], (int)data[x][y]);
             }
         }


         temp_wximage->Rescale(scale, scale, wxIMAGE_QUALITY_HIGH);

         wxBitmap * temp_wxbitmap = new wxBitmap(*temp_wximage);

         dest->SetBitmap(*temp_wxbitmap);

         delete temp_wximage;
         delete temp_wxbitmap;
    }

    void set_image(wxStaticBitmap * dest, int scale, float RH, float GH, float BH, int RL, int GL, int BL)
    {
        wxImage * temp_wximage = new wxImage(size, size, true);

        float RC, GC, BC;

        RC = (RH - RL) / 255.0;
        GC = (GH - GL) / 255.0;
        BC = (BH - BL) / 255.0;

         for (int x = 0; x < size; x++)
         {
             for (int y = 0; y < size; y ++)
             {
                 temp_wximage->SetRGB(x, y, (int)(data[x][y] * RC + RL) ,(int)(data[x][y] * GC + GL), (int)(data[x][y] * BC + BL));
             }
         }


         temp_wximage->Rescale(scale, scale, wxIMAGE_QUALITY_HIGH);

         wxBitmap * temp_wxbitmap = new wxBitmap(*temp_wximage);

         dest->SetBitmap(*temp_wxbitmap);

         delete temp_wximage;
         delete temp_wxbitmap;
    }

    ~greyscale()
    {
        for (int i = 0; i < size; i++) delete[] data[i];
        delete[] data;
    }
};

