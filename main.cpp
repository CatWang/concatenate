#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>

using namespace std;
using namespace cv;

vector<string> split(const string &s, char delim)
{
    stringstream ss;
    ss.str(s);
    string item;
    vector<string> result;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

vector<string> getAllValidImages(string img_list)
{
    vector<string> validImages;
    ifstream img_list_file(img_list);
    if (img_list_file.is_open())
    {
        string line;
        while (getline(img_list_file, line))
        {
            vector<string> result = split(line, '\"');
            // 这里假设图片被标为1时是删除，0是保留
            if (result.size() != 2)
            {
                cout << "ERROR: at split!!!";
                exit(-1);
            }
            string flag = result[1];
            if (flag == "删除")
            {
                continue;
            } else if (flag == "保留")
            {
                validImages.push_back(result[0]);

            } else
            {
                cout << "ERROR: The index of image " << result[0] << " is not in standard format!" << endl;
                continue;
            }
        }
    }
    return validImages;
}

string getName(string image_path)
{
    vector<string> directories = split(image_path, '/');
    string name = "";
    for (int i = 0; i < directories.size() - 2; ++i) {
        name = name + directories[i] + '_';
    }
    name += directories[directories.size() - 2];
    return name;
}

vector<int> get8Index(int size)
{
    vector<int> index;

    int count = 0;

    while (count < 8)
    {
        int randomIndex = rand() % size;
        if (find(index.begin(), index.end(), randomIndex) != index.end())
            continue;

        index.push_back(randomIndex);
        count++;
    }

    return index;
}

vector<string> get8Path(vector<string> current_images, string input_root)
{
    vector<string> path8;

    if (current_images.size() <= 8)
    {
        while (8 - path8.size() >= current_images.size())
        {
            for (int i = 0; i < current_images.size(); ++i) {
                path8.push_back(input_root + current_images[i]);
            }
        }

        int current_size = path8.size();

        for (int j = 0; j < 8 - current_size; ++j) {
            path8.push_back(input_root + current_images[j]);
        }
    }
    else
    {
        vector<int> index = get8Index(current_images.size());
        for (int i = 0; i < index.size(); ++i) {
            path8.push_back(input_root + current_images[index[i]]);
        }
    }

    return path8;
}

int concat(string img_list, string input_root, string output_root)
{
    vector<string> valid_images = getAllValidImages(img_list);

    string result_path = output_root + "concat_imglist.txt";

    ofstream result_file(result_path);

    vector<string> current_images, done_images;

    string current_name;
    //int count = 0;

    for (int i = 0; i < valid_images.size(); i ++)
    {
        string image_path = valid_images[i];
        string name = getName(image_path);

        if (find(done_images.begin(), done_images.end(), name) != done_images.end())
            continue;

        done_images.push_back(name);
        current_images.push_back(image_path);
        current_name = name;

        for (int j = i + 1; j < valid_images.size(); ++j) {
            string temp_image = valid_images[j];
            string temp_name = getName(temp_image);

            if (temp_name != current_name)
            {
//                cout << "temp_name: " << endl;
//                cout << "current_name: " << endl;
                break;

                //如果结果不相临
                //continue;
            }
            cout << "temp image" << temp_image << endl;
            current_images.push_back(temp_image);
            //count++;
        }

        vector<string> path8 = get8Path(current_images, input_root);

        for (int k = 0; k < path8.size(); ++k) {
            cout << path8[k] << endl;
        }
        Mat combine, combine1, combine2, combine3, combine4, combine5, combine6;
        Mat image1 = imread(path8[0]);
        Mat image2 = imread(path8[1]);
        Mat image3 = imread(path8[2]);
        Mat image4 = imread(path8[3]);
        Mat image5 = imread(path8[4]);
        Mat image6 = imread(path8[5]);
        Mat image7 = imread(path8[6]);
        Mat image8 = imread(path8[7]);


        hconcat(image1, image2, combine1);
        hconcat(combine1, image3, combine2);
        hconcat(combine2, image4, combine3);

        hconcat(image5, image6, combine4);
        hconcat(combine4, image7, combine5);
        hconcat(combine5, image8, combine6);

        vconcat(combine3, combine6, combine);

        imwrite(output_root + current_name + ".jpg", combine);
        result_file << current_name + ".jpg" << endl;
        // 记得保存和清空这些vector～～
        current_images.clear();
        //count = 0;
    }

    result_file.close();
}


int main()
{
    cout << "删除后图片路径的文件(raw data): ";
    string img_list;
    cin >> img_list;

    cout << "这些路径是针对哪个根目录呢: ";
    string input_root;
    cin >> input_root;

    cout << "渲染好的图片你想让我放到哪里： ";
    string output_root;
    cin >> output_root;

    concat(img_list, input_root, output_root);

    return 0;
}