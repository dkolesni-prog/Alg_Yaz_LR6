#include <algorithm>

#include <fstream>

#include <iostream>

#include <string>

#include <vector>
//Определите структуру «Житель». Поля: ФИО, переменная структурного типа «адрес» (поля: улица,
//номер дома, номер квартиры), пол, возраст. В программе задайте вектор из нескольких жителей –
//фрагмент базы данных ЖЭК (массив переменных структурного типа).
//Отсортируйте базу по ФИО лиц и определите, сколько лиц женского пола в возрасте старше 30 лет
//        проживают в заданном доме (вводится с клавиатуры, вывести значение и список объектов).
//Определите функции записи базы в файл и чтения базы из файла в текстовом и бинарном режимах.
//Функции, работающие в текстовом режиме, определите как перегрузки операторов >> и <<.

//ЛР6:
//1. Всё-таки ФИО содержит пробелы, так что читать через cin » — плохая идея
//2. Не перегружен оператор » для чтения вектора из файла
//3. Не работает чтение и запись в бинарном режиме. Не в последнюю очередь из-за того что sizeof() для вектора возвращает не размер базы в байтах. Здесь не всё так просто :)

//Если вы откроете "my_zhek_bin.txt", увидите, что данных там сильно меньше, чем должно быть. Есть иллюзия работы, т.к. вектор уже заполнен данными. Если закомментить всё, кроме чтения из файла, увидите, что обратно вектор не воспроизводится.
//И не забудьте, что для бинарного режима тоже должны быть определены функции.

struct Address{
    std::string street;
    int building;
    int apartment;
};

void showStreetAddr(Address a) {
    std::cout << a.street << std::endl;
}

struct Citizen{
    std::string initials;
    Address place;
    std::string gender;
    int age;
};

void init_zhek (std::vector <Citizen>& vec ,int size){
    for (int i = 0; i < size; ++i) {
        Citizen tmp;
        std::cout << "Enter initials: " << std::endl;
        getline(std::cin, tmp.initials);
        std::cout << "Enter street: " << std::endl;
        getline(std::cin, tmp.place.street);
        std::cout << "Enter building number: " << std::endl;
        std::cin >> tmp.place.building;
        std::cout << "Enter apt: " << std::endl;
        std::cin >> tmp.place.apartment;
        std::cout << "Enter gender: " << std::endl;
        std::cin >> tmp.gender;
        std::cout << "Enter age: " << std::endl;
        std::cin >> tmp.age;
        vec.push_back(tmp);

    }
}

void show_zhek (std::vector <Citizen>& vec){
    std::for_each(vec.begin(),vec.end(),[](Citizen tmp){
        std::cout << "Initials: ";
        std::cout << tmp.initials << std::endl;
        std::cout << "Street: ";
        std::cout << tmp.place.street << std::endl;
        std::cout << "Building: ";
        std::cout << tmp.place.building << std::endl;
        std::cout << "Apt: ";
        std::cout << tmp.place.apartment << std::endl;
        std::cout << "Gender: ";
        std::cout << tmp.gender << std::endl;
        std::cout << "Age: ";
        std::cout << tmp.age << std::endl;
        std::cout << "----------- " << std::endl;
    });
}



void show_citizen (Citizen& tmp){
    std::cout << "Initials: ";
    std::cout << tmp.initials << std::endl;
    std::cout << "Street: ";
    std::cout << tmp.place.street << std::endl;
    std::cout << "Building: ";
    std::cout << tmp.place.building << std::endl;
    std::cout << "Apt: ";
    std::cout << tmp.place.apartment << std::endl;
    std::cout << "Gender: ";
    std::cout << tmp.gender << std::endl;
    std::cout << "Age: ";
    std::cout << tmp.age << std::endl;
    std::cout << "----------- " << std::endl;

}


std::ostream& operator<<(std::ostream& out, const std::vector<Citizen>& vec)
{
    for (auto& tmp : vec){
        out << "Initials: ";
        out << tmp.initials << std::endl;
        out << "Street: ";
        out << tmp.place.street << std::endl;
        out << "Building: ";
        out << tmp.place.building << std::endl;
        out << "Apt: ";
        out << tmp.place.apartment << std::endl;
        out << "Gender: ";
        out << tmp.gender << std::endl;
        out << "Age: ";
        out << tmp.age << std::endl;
        out << "----------- " << std::endl;
    }
    return out;
}

std::istream& operator>>(std::istream& from, std::vector<Citizen>& vec)
{
    std::string content;
    while (std::getline(from, content)){
        Citizen tmp;
        tmp.initials = content;

        std::getline(from, content);
        tmp.place.street = content;

        std::getline(from, content);
        tmp.place.building = std::stoi(content);

        std::getline(from, content);
        tmp.place.apartment = std::stoi(content);

        std::getline(from, content);
        tmp.gender = content;

        std::getline(from, content);
        tmp.age = std::stoi(content);

        std::getline(from, content);// разделитель ----------
        vec.push_back(tmp);
    }

    return from;
}

void bin_write (std::ostream& to, std::vector<Citizen>& vec){
    Citizen tmp;
    while (!to.eof()) {
        to.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
        vec.push_back(tmp);
    }
}

void bin_read (std::istream& to, std::vector<Citizen>& vec){
    for (int i = 0; i < vec.size(); ++i) {
        to.read(reinterpret_cast<char*>(&vec[i]), sizeof(vec[i]));
    }
}

int main() {
    std::vector<Citizen> my_zhek;
    init_zhek(my_zhek, 2);
    show_zhek(my_zhek);
    std::sort(my_zhek.begin(),my_zhek.end(), [](Citizen& i, Citizen& j){return  i.initials < j.initials;});
    show_zhek(my_zhek);
    std::string street_;
    int building_ = 0;
    int counter = 0;
    std::cout << "Enter street as an argument to search by" << std::endl;
    std::cin >> street_;
    std::cout << "Enter building as an argument to search by" << std::endl;
    std::cin >> building_;

    counter = std::count_if(my_zhek.begin(), my_zhek.end(),[building_, street_](Citizen& element){if ((element.age > 30)
                                                                                                      && (element.place.building == building_) && (element.place.street == street_) && element.gender == "F"){show_citizen( element);return true;}return false;});
    std::cout << "There are " << counter << " citizens over 30 in " << building_ << " 'th building";

    std::ifstream from;
    from.open("my_zhek.txt");
    if (from.is_open()) {
        from >> my_zhek;
        from.close();
    }

    std::ofstream out;
    out.open("my_zhek.txt");
    if (out.is_open()) {
        out << my_zhek;
        out.close();
    }

    std::ofstream bin_in ("my_zhek_bin.txt", std::ios::binary);
    if(bin_in.is_open()) {
        bin_write(bin_in, my_zhek);
        bin_in.close();
    }

    std::ifstream bin_out ("my_zhek_bin.txt", std::ios::binary);
    if (bin_out.is_open()) {
        bin_read(bin_out, my_zhek);
        bin_out.close();
    }

    bin_out.close();

    return 0;
}
