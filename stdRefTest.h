#include <iostream>
#include <vector>
#include <memory>
#include <array>

#define CHECK_COUNT(CONTAINER, EXPECTEDSIZE) if(EXPECTEDSIZE != CONTAINER.size())\
std::cout << "WRONG count in" << #CONTAINER << std::endl;\
else std::cout << "OK. Response tag count in " << #CONTAINER << ": " << EXPECTEDSIZE << std::endl;


class TagContainer
{
public:
	enum TagType
	{
		TagRequest,
		TagResponse
	};
private:
	TagType m_tagType;
	static constexpr auto VALUE_SIZE = 1024;
	std::array<char, VALUE_SIZE> m_value;
public:

	TagContainer(TagType tagType) : m_tagType(tagType) {};

	TagType getType() { return m_tagType; };
	void SetData(char data, unsigned short index)
	{
		m_value[index] = data;
	}
};

using TagsVector = std::vector<std::reference_wrapper<TagContainer>>;

class TagFilter
{
public:

	TagsVector getRequestTags(std::vector<std::unique_ptr<TagContainer>> &Tags)
	{
		TagsVector returnData;
		auto filterRule = [&returnData](auto &Tags)
		{
			if (Tags->getType() == TagContainer::TagRequest)
				returnData.emplace_back(std::ref(*Tags.get()));
		};
		std::for_each(std::begin(Tags), std::end(Tags), filterRule);
		return returnData;
	}

	TagsVector getResponseTags(std::vector<std::unique_ptr<TagContainer>> &Tags)
	{
		TagsVector returnData;
		auto filterRule = [&returnData](auto &Tags)
		{
			if (Tags->getType() == TagContainer::TagResponse)
				returnData.emplace_back(std::ref(*Tags.get()));
		};
		std::for_each(std::begin(Tags), std::end(Tags), filterRule);
		return returnData;
	};

};


void filterTest()
{
	std::vector<std::unique_ptr<TagContainer>> testValues;

	// fill vector
	static constexpr auto requestCount = 10;
	static constexpr auto responseCount = 20;
	for (auto i = 0; i != requestCount; ++i)
		testValues.emplace_back(new TagContainer(TagContainer::TagRequest));
	for (auto i = 0; i != responseCount; ++i)
		testValues.emplace_back(new TagContainer(TagContainer::TagResponse));

	// create our "filter" class
	auto filter = std::make_unique<TagFilter>();

	// filter it
	auto requestTag = filter->getRequestTags(testValues); // requestTag and responseTag are vectors with references to items from testValues 
	auto responseTag = filter->getResponseTags(testValues);

	CHECK_COUNT(requestTag, requestCount);
	CHECK_COUNT(responseTag, responseCount);
}