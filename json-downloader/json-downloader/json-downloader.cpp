// json-downloader.cpp
//

#include "pch.h"

std::shared_ptr<spdlog::logger> console = spdlog::stdout_color_mt("console");

int download(std::string url, std::string filename);

int main(int argc, char* argv[])
{
	if ( console == nullptr )
	{
		console = spdlog::stdout_color_mt("console");
	}

	if ( argc < 2 )
	{
		console->info("Usage: {0} <JSON FILE>\n", argv[0] );
		console->info("Format of JSON: { URL1 : LocalFileName1, URL2 : LocalFileName2 }\n");
		return 1;
	}

	std::string jsonFilename(argv[1]);
	std::ifstream jsonFile( jsonFilename );
	if (!jsonFile.is_open())
	{
		console->error( "Failed to load {0}", jsonFilename );
		return (-1);
	}

	json downloadListJson;

	try
	{
		downloadListJson = json::parse(jsonFile);
	}
	catch (json::parse_error& e)
	{
		console->error( "{0}", e.what() );
		return (-2);
	}

	curl_global_init(CURL_GLOBAL_ALL);

	for (auto& x : downloadListJson.items())
	{
		std::string urlValue = x.key();
		std::string localFilenameValue = x.value(); 

		int ret = download( urlValue, localFilenameValue);
		if (ret != 0) 
		{
			console->error( "Failed to download : {0}", urlValue );
		}
	}

	// TODO: extract compressed files 

	curl_global_cleanup();

	return 0; 
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	console->info(" {0}, {1}", size, nmemb );
	return written;
}

int download(std::string url, std::string filename)
{
	CURL* curl_handle = curl_easy_init();

	curl_easy_setopt( curl_handle, CURLOPT_URL, url.c_str() );
	curl_easy_setopt( curl_handle, CURLOPT_VERBOSE, 1L );
	curl_easy_setopt( curl_handle, CURLOPT_NOPROGRESS, 1L );
	curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, write_data );

	FILE* pagefile = fopen( filename.c_str(), "wb" );
	if ( !pagefile )
	{
		console->error( "Failed to create file : {0}", filename );
		curl_easy_cleanup(curl_handle);
		return (-1);
	}

	curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, pagefile );

	console->info( "Download : {0}", url );
	curl_easy_perform( curl_handle );
	fclose( pagefile );
	console->info( "Success to download : {0}", url );

	curl_easy_cleanup(curl_handle);
	return 0;
}
