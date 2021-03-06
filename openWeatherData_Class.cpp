#include "openWeatherData_Class.h"

//requestCurrentWeather::requestCurrentWeather(){};

using json = nlohmann::json;
//requestCurrentWeather::requestCurrentWeather():client(http_client(U("http://api.openweathermap.org/data/2.5/weather?"))) {};

long long getEpochTime() //grabs the current UNIX time
{
	const long long timeNow = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
	return timeNow;
}

tmillis_t GetTimeInMillis()
{
	struct timeval tp;
	gettimeofday(&tp, nullptr);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int requestCurrentWeather::getWeatherData()
{
	//cURLpp::initialize(CURL_GLOBAL_ALL);
	
	std::stringstream jsonResponse;
	
	try
	{
		
		std::string baseURL = "http://api.openweathermap.org/data/2.5/weather?";
		
		std::string url = (baseURL).append("zip=").append(location).append("&units=").append(getRequestTempUnits).append("&appid=").append(apiKey); 
				
		curlpp::Easy myRequest;
		
		myRequest.setOpt(new curlpp::options::Url(url));
		curlpp::options::WriteStream ws(&jsonResponse);
		myRequest.setOpt(ws);

		json weatherJSON_Response;
			
		myRequest.perform();
		ws.clear();
		int statusCode = curlpp::infos::ResponseCode::get(myRequest);
		if (statusCode == 200)
		{
			//printf("Received response status code:%u\n", response.status_code());
			weatherJSON_Response = json::parse(jsonResponse); 
			jsonResponse.str("");
			json weather = weatherJSON_Response.at("weather");
			json main = weatherJSON_Response.at("main");
			json sys = weatherJSON_Response.at("sys");
			json wind = weatherJSON_Response.at("wind");
			this->arrayOfWeatherIDs.clear();
				             
			//in the format [currentTime, sunRise, sunSet]
		    
			
			for(auto i : weather[0].at("id"))
			{
				int weatherID = int(i);
				//std::cout << weatherID << std::endl;
				this->arrayOfWeatherIDs.push_back(weatherID);
				//std::wcout << i.at(U("id")).as_integer() << "\n";
			}
			
				             
			//this->arrayOfWeatherIDs.push_back(600);
			
			this->currentTemperature = int(main.at("temp"));
			this->feelsLikeTemp = int(main.at("feels_like"));
			this->windSpeed = int(wind.at("speed"));
			int64_t sunrise = long(sys.at("sunrise"));
			int64_t sunset = long(sys.at("sunset"));
			this->arrayOfTimes.clear();
			this->arrayOfTimes.push_back(getEpochTime());
			this->arrayOfTimes.push_back(sunrise);
			this->arrayOfTimes.push_back(sunset);
			this->arrayOfTimesRef = &(this->arrayOfTimes);
			this->arrayOfWeatherIDsRef = &(this->arrayOfWeatherIDs);
			
			weatherJSON_Response.clear();
			myRequest.reset();
			//cURLpp::terminate();

		}
		
	}
	catch (curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}

	catch (curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
	
}
	/*
	try
	{
	
		// Build request URI and start the request.
		//http_client client(U("http://api.openweathermap.org/data/2.5/weather?"));
		//uri_builder builder;
		//json::value openWeatherJSONResponse;
		builder.clear();
		builder.append_query(U("zip"), location);
		builder.append_query(U("appid"), apiKey);
		builder.append_query(U("units"), getRequestTempUnits);
		return client.request(methods::GET, builder.to_string())
		            // Handle response headers arriving.
		             .then([&](http_response response)

		             {
			           
	
			             if (response.status_code() == status_codes::OK)
			             {
				             //printf("Received response status code:%u\n", response.status_code());
				             weatherJSON_Response = json::parse(jsonResponse);
				             json weather = weatherJSON_Response.at("weather");
				             json main = weatherJSON_Response.at("main");
				             json sys = weatherJSON_Response.at("sys");
				             json wind = weatherJSON_Response.at("wind");
				             //openWeatherJSONResponse = response.extract_json().get();
				             //json::value weather = openWeatherJSONResponse.at(U("weather"));
				             //json::value main = openWeatherJSONResponse.at(U("main"));
				             //json::value sys = openWeatherJSONResponse.at(U("sys"));
				             //json::value wind = openWeatherJSONResponse.at(U("wind"));
				             this->arrayOfWeatherIDs.clear();
				             
				             //in the format [currentTime, sunRise, sunSet]
				            
				             for (auto i : weather[0])
				             {
					             this->arrayOfWeatherIDs.push_back(int(i.at("id")));
					             //std::wcout << i.at(U("id")).as_integer() << "\n";
				             }
				             
				             
				            // this->arrayOfWeatherIDs.push_back(741);

				             this->currentTemperature = main.at(U("temp")).as_integer();
				             this->feelsLikeTemp = main.at(U("feels_like")).as_integer();
				             this->windSpeed = wind.at(U("speed")).as_integer();
				             int64_t sunrise = sys.at(U("sunrise")).as_number().to_int64();
				             int64_t sunset = sys.at(U("sunset")).as_number().to_int64();
				             this->arrayOfTimes.clear();
				             this->arrayOfTimes.push_back(getEpochTime());
				             this->arrayOfTimes.push_back(sunrise);
				             this->arrayOfTimes.push_back(sunset);
				             this->arrayOfTimesRef = &(this->arrayOfTimes);
				             this->arrayOfWeatherIDsRef = &(this->arrayOfWeatherIDs);
				             
			             }
		             }).wait();
	}
	
	
		// Wait for all the outstanding I/O to complete and handle any exceptions

	catch (const std::exception& e)
	{
		clearImageRenderList();
		//imageRenderListPush("./error_icon.png");
	}
}
*/
int requestCurrentWeather::getCurrentTemperature() const
{
	return currentTemperature;
}

vector<int>* requestCurrentWeather::getWeatherIDArray() const
{
	return arrayOfWeatherIDsRef;
}

int requestCurrentWeather::getWindSpeed() const
{
	return windSpeed;
}

int requestCurrentWeather::getFeelsLikeTemp() const
{
	return feelsLikeTemp;
}

vector<long long>* requestCurrentWeather::getTimeArray() const
{
	return arrayOfTimesRef;
}

time_t requestCurrentWeather::getTime() const
{
	return counter;
}

void requestCurrentWeather::imageRenderListPush(string i)
{
	imageRenderList.push_back(i);
}

void requestCurrentWeather::imageRenderListPop()
{
	imageRenderList.erase(imageRenderList.begin());
}

vector<string> requestCurrentWeather::getImageRenderList() const
{
	return imageRenderList;
}

int requestCurrentWeather::getWeatherArrayFirstElement()
{
	return arrayOfWeatherIDs[0];
}

int requestCurrentWeather::getPrecipitationIntensity(int weatherCode)
{
	enum intensity
	{
		light,
		medium,
		heavy,
		extreme
		
	};
	
	switch (weatherCode) {
		
	case 300:
		
		return light;
		
	case 301:
		
		return light;
		
	case 302:
		
		return medium;
		
	case 310:
		
		return light;
	
	case 311:
		
		return light;
		
	case 312:
		
		return medium;
	
	case 313:
		
		return light;
	
	case 314:
		
		return medium; 
	
	case 321: 
		
		return light;

	case 500:
		
		return light;
		
	case 501: 
		
		return medium;
	
	case 502: 
		
		return heavy;
	
	case 503:
		
		return extreme;
	
	case 504:
		
		return extreme;
		
	case 511:
		
		return medium;
		
	case 520:
		
		return light;
		
	case 521:
		
		return medium;
			
	case 522:
	
		return heavy;
			
	case 531:
			
		return medium;
		
	case 600:
		
		return light;
			
	case 601:
		
		return medium;
		
	case 602:
		
		return heavy;
	
	case 611:
		
		return medium;
	
	case 612:
		
		return light;
		
	case 613:
		
		return medium;
		
	case 615:
		
		return light;
	
	case 616:
		
		return medium;
		
	case 620:
		
		return light;
		
	case 621:
			
		return medium;
	
	case 622:
		
		return heavy;
			
		
	}
}



int requestCurrentWeather::getPrecipitationType(int weatherCode)
{
	enum precipitationTypes
	{
		none,
		rain,
		snow,
		sleet
	};
	
	switch (weatherCode)
	{
	case 200 ... 521:
		return rain;
	case 600 ... 602:
		return snow;
	case 611 ... 621:
		return sleet;
	case 622:
		return snow;
	case 701 ... 804:
		return none;
	
	}	
}


void requestCurrentWeather::clearImageRenderList()
{
	imageRenderList.clear();
}

void requestCurrentWeather::setLastImageRendered(string fileName)
{
	lastImageRenderedName = fileName;
}

string requestCurrentWeather::getLastImageRendered() const
{
	return lastImageRenderedName;
}

void requestCurrentWeather::setLastFile_Img(vector<FileInfo*> file)
{
	file_imgs = file;
}

vector<FileInfo*> requestCurrentWeather::getLastFile_Img()
{
	return file_imgs;
}

void requestCurrentWeather::initOpenWeatherOptions(weatherAPIOptions* initWeatherOptions)
{
	this->apiKey = initWeatherOptions->apiKey;
	this->getRequestTempUnits = initWeatherOptions->getRequestTempUnits;
	this->location = initWeatherOptions->location;
	this->currentTemperature = initWeatherOptions->currentTemperature;
	this->arrayOfWeatherIDs = initWeatherOptions->arrayOfWeatherIDs;
	this->arrayOfTimes = initWeatherOptions->arrayOfTimes;
	this->file_imgs = initWeatherOptions->file_imgs;
	this->windSpeed = initWeatherOptions->windSpeed;
	this->feelsLikeTemp = initWeatherOptions->feelsLikeTemp;
	this->counter = initWeatherOptions->counter;
	this->timeout = initWeatherOptions->timeout;
	this->lastImageRenderedName = initWeatherOptions->lastImageRenderedName;
	this->imageRenderList = initWeatherOptions->imageRenderList;
}

void requestCurrentWeather::setPrecipImageStatus(bool status)
{
	
	*precipStatusRef = status;
	
}

bool requestCurrentWeather::getPrecipImageStatus()
{
	return *precipStatusRef;
}


