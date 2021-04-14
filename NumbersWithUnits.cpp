#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace ariel
{
    double NumberWithUnits::getNum()
    {
        return num;
    }
    string NumberWithUnits::getUnit()
    {
        return unit;
    }

    // static void read_units(ifstream &file)
    // {
    //     if (file.is_open())
    //     {
    //         std::string s;
    //         while (file >> s)
    //         {
    //             NumberWithUnits::myUnits.push_back(s);
    //         }
    //     }
    // }

    void NumberWithUnits::initGraph(vector<string> &units, Graph &graph)
    {
        //list of units inserted up to now
        vector<pair<string, int>> graph_units;

        for (unsigned int i = 0; i < units.size(); i += 5)
        {
            std::string unit1 = units.at(i + 1);
            std::string unit2 = units.at(i + 4);

            if (graph.umap.count(units.at(i + 1)) == 0 && graph.umap.count(units.at(i + 4)) == 0)
            {
                insert2units(units, graph, i);
            }
            else if (graph.umap.count(units.at(i + 1)) == 0 || graph.umap.count(units.at(i + 4)) == 0)
            {
                insert1unit(units, graph, i);
            }
        }
    }

    void NumberWithUnits::print(const Graph &graph)
    {
        string s = "[";
        vector<vector<pair<string, vector<pair<string, double>>>>> vec = graph.matrix;

        for (unsigned int i = 0; i < vec.size(); i++)
        {
            s += "Line:" + to_string(i) + " ";
            for (unsigned int j = 0; j < vec.at(i).size(); j++)
            {
                s += "<" + vec.at(i).at(j).first + "::";
                for (unsigned int k = 0; k < vec.at(i).at(j).second.size(); k++)
                {
                    s += "(" + vec.at(i).at(j).second.at(k).first + "," + to_string(vec.at(i).at(j).second.at(k).second) + ")";
                }
                if (j != vec.at(i).size() - 1)
                    s += ">,";
                else
                    s += ">";
            }
            if (i == vec.size() - 1)
            {
                s += "]";
            }
            else
            {
                s += "\n";
            }
        }
        cout << s << endl;
    }
    int NumberWithUnits::findIndex(Graph &graph, string unit)
    {
        int line = graph.umap.at(unit);
        for (size_t i = 0; i < graph.matrix.at(line).size(); i++)
        {
            if (graph.matrix.at(line).at(i).first == unit)
            {
                return i;
            }
        }
        return -1;
    }
    double NumberWithUnits::connect(Graph &graph, string left_unit, string right_unit)
    {
        double conversion;
        unordered_map<string, int> existing_units;
        existing_units[left_unit] = 0;
        try
        {
            if (graph.umap.at(left_unit) == graph.umap.at(right_unit))
            {
                int line = graph.umap.at(left_unit);
                vector<pair<string, vector<pair<string, double>>>> &ver_vec = graph.matrix.at(line);

                int ind = findIndex(graph, left_unit);
                vector<pair<string, double>> &vec = ver_vec.at(ind).second; //KM_VEC
                unsigned int length = vec.size();                           //KM vector length

                int vec_length = vec.size();
                for (unsigned int i = 0; i < ver_vec.size(); i++)
                {
                    string temp_unit = vec.at(i).first; // looking for the same unit
                    if (temp_unit == right_unit)
                    {
                        return vec.at(i).second;
                    }
                    conversion = vec.at(i).second;

                    int temp_ind = findIndex(graph, temp_unit);
                    vector<pair<string, double>> &vec_temp = ver_vec.at(temp_ind).second;
                    unsigned int temp_length = vec_temp.size();

                    for (unsigned int j = 0; j < temp_length; j++)
                    {
                        string temp_name = vec_temp.at(j).first;
                        if (existing_units.find(temp_name) == existing_units.end())
                        {
                            conversion *= vec_temp.at(j).second;
                            vec.push_back(make_pair(temp_name, conversion));
                            existing_units[temp_name] = i;
                            vec_length++;
                            conversion /= vec_temp.at(j).second;
                        }
                    }
                }
                for (unsigned int i = 0; i < vec.size(); i++)
                {
                    if (vec.at(i).first == right_unit)
                    {
                        return vec.at(i).second;
                    }
                }
            }
            else
            {
                throw(make_pair(left_unit, right_unit));
            }
        }
        catch (pair<string, string> units)
        {
            cout << units.first << " is not from unit-verse of " << units.second << endl;
        }
        return conversion;
    }
    void NumberWithUnits::insert1unit(vector<string> &units, Graph &graph, int i)
    {
        int pointer;
        double conv1 = stod(units.at(i + 3));
        string unit1 = units.at(i + 1);
        string unit2 = units.at(i + 4);
        pair<string, int> left_unit;
        pair<string, int> right_unit;
        pair<string, vector<pair<string, double>>> add_to_existing_vec; // add new unit to line in matrix
        vector<pair<string, double>> add_to_pair;                       // vec inside the pair

        //if the right unit is missing:
        if (graph.umap.count(units.at(i + 4)) == 0)
        {
            pointer = graph.umap.at(unit1); // at which line am I
        }

        //if the left unit is missing
        else
        {
            unit1 = units.at(i + 4);
            unit2 = units.at(i + 1);
            //find where at which line am i
            pointer = graph.umap.at(unit1);
            conv1 = 1 / stod(units.at(i + 3));
        }

        pair<string, double> curr_conversion(unit1, 1 / conv1); //a pair that will be added
        add_to_pair.push_back(curr_conversion);                 // created a new conversion list and added the curr conversion

        //give the new vector a name
        add_to_existing_vec.first = unit2;
        add_to_existing_vec.second = add_to_pair;

        //push to the matrix the final vector
        graph.matrix.at(pointer).push_back(add_to_existing_vec); // add pair to the existing vecto

        //create pair to add to the existing vector of the other unit
        left_unit.first = unit2;
        left_unit.second = pointer;

        int ind;
        for (unsigned int i = 0; i < graph.matrix.at(pointer).size(); i++)
        {
            if (graph.matrix.at(pointer).at(i).first == unit1)
                ind = i;
        }

        pair<string, double> left_conversion(unit2, conv1); //(m,1000)
        graph.matrix.at(pointer).at(ind).second.push_back(left_conversion);
        graph.umap.insert(left_unit);
    }
    void NumberWithUnits::insert2units(vector<string> &units, Graph &graph, int i)
    {
        double conv1 = stod(units.at(i + 3));
        double conv2 = 1 / stod(units.at(i + 3));
        string unit1 = units.at(i + 1);
        string unit2 = units.at(i + 4);

        //this is the vector we eventually emplace back to the matrix
        vector<pair<string, vector<pair<string, double>>>> final_vec;

        //vector that will contain conversions
        vector<pair<string, double>> right_inside_units;
        vector<pair<string, double>> left_inside_units;

        //each conversion going inside above vector
        pair<string, double> conversion;
        conv1 = stod(units.at(i + 3));
        conv2 = 1 / stod(units.at(i + 3));

        pair<string, double> right_conversion(units.at(i + 1), conv2); // (km,1\1000)

        pair<string, double> left_conversion(units.at(i + 4), conv1); //(m,1000)
        left_inside_units.push_back(left_conversion);
        right_inside_units.push_back(right_conversion);

        //this pair is one of two that will be pushed inside above vec
        pair<string, vector<pair<string, double>>> left_pair(units.at(i + 1), left_inside_units);
        pair<string, vector<pair<string, double>>> right_pair(units.at(i + 4), right_inside_units);

        final_vec.push_back(left_pair);
        final_vec.push_back(right_pair);

        //add new conversion units to the graph
        graph.matrix.emplace_back(final_vec);

        //mark which units were added to the list with the index of the line
        pair<string, int> left_unit(units.at(i + 1), graph.matrix.size() - 1);
        pair<string, int> right_unit(units.at(i + 4), graph.matrix.size() - 1);
        graph.umap.insert(left_unit);
        graph.umap.insert(right_unit);
    }

    NumberWithUnits operator+(const NumberWithUnits &f1, const NumberWithUnits &f2)
    {
    }
    NumberWithUnits operator+=(const NumberWithUnits &f1, const NumberWithUnits &f2)
    {
    }
    NumberWithUnits operator+(const NumberWithUnits &f1)
    {

    } //plus unari

    NumberWithUnits operator-(const NumberWithUnits &f1, const NumberWithUnits &f2)
    {
    }
    NumberWithUnits operator-=(const NumberWithUnits &f1, const NumberWithUnits &f2)
    {
    }
    NumberWithUnits operator-(const NumberWithUnits &f1)
    {
    }

    bool NumberWithUnits::operator<(const NumberWithUnits &f) const
    {
    }
    bool NumberWithUnits::operator<=(const NumberWithUnits &f) const
    {
    }
    bool NumberWithUnits::operator>(const NumberWithUnits &f) const
    {
    }
    bool NumberWithUnits::operator>=(const NumberWithUnits &f) const
    {
    }
    bool NumberWithUnits::operator==(const NumberWithUnits &f) const
    {
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits &f) const
    {
    }

    NumberWithUnits &NumberWithUnits::operator++()
    {

    } // prefix: ++a
    const NumberWithUnits NumberWithUnits::operator++(int)
    {

    } // postfix: a++

    NumberWithUnits &NumberWithUnits::operator--()
    {

    } // prefix: --a
    const NumberWithUnits NumberWithUnits::operator--(int)
    {

    } // postfix: a--

    NumberWithUnits operator*(const NumberWithUnits &f1, const double &f2)
    {
    }
    NumberWithUnits operator*(const double &f1, const NumberWithUnits &f2)
    {
    }

    std::ostream &operator<<(std::ostream &os, const NumberWithUnits &f)
    {
    }
    std::istream &operator>>(std::istream &is, NumberWithUnits &f)
    {
    }
}