#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
using namespace std;
using namespace Json;
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp)
{
    size_t totalSize = size * nmemb;
    *userp += string((char*)contents, totalSize);
    return totalSize;
}

string encodedcity(const string& city)
{
    string City = city;
    size_t pos = 0;
    while ((pos = City.find(' ', pos)) < City.length())
    {
        City.replace(pos, 1, "%20");
        pos += 3;
    }
    return City;
}
void getWeather(const string& city, const string& apiKey)
{
    CURL* curl;
    CURLcode res;
    string readBuffer;
    string encodedCity = encodedcity(city);
    string url = "http://api.openweathermap.org/data/2.5/weather?q=" + encodedCity + "&appid=" + apiKey + "&units=metric";

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK)
        {
            cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return;
        }
        Value jsonData;
        CharReaderBuilder reader;
        string errs;

        istringstream read(readBuffer);
        if (parseFromStream(reader, read, &jsonData, &errs))
        {
            cout << "City: " << jsonData["name"].asString() << endl;
            cout << "Temperature: " << jsonData["main"]["temp"].asFloat() << " C" << endl;
            cout << "Humidity: " << jsonData["main"]["humidity"].asInt() << " %" << endl;
            cout << "Weather: " << jsonData["weather"][0]["description"].asString() << endl;
            cout << "Wind Speed: " << jsonData["wind"]["speed"].asFloat() << " m/s" << endl;
            if (jsonData.isMember("rain"))
            {
                cout << "Rain (last 1 hour): " << jsonData["rain"]["1h"].asFloat() << " mm" << endl;
            }
            else
            {
                cout << "Rain: 0 mm" << endl;
            }


            if (jsonData.isMember("snow"))
            {
                cout << "Snow (last 1 hour): " << jsonData["snow"]["1h"].asFloat() << " mm" << endl;
            }
            else
            {
                cout << "Snow: 0 mm" << endl;
            }
        }
        else
        {
            cout << "Failed to parse JSON: " << errs << endl;
        }
    }
}
int main()
{
    cout << "\t\t\t\"Weather Forecating App\"\t\t\t\n";
    string city;
    string apiKey = "cb8440c27547743075b4bb259db63383";
    cout << "Enter city name: ";
    getline(cin, city);
    getWeather(city, apiKey);
    return 0;
}










