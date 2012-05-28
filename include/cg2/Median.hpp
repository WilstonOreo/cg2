#pragma once

#include <vector>

using std::vector;


template <typename T, class Compare >
static T quick_select(const vector<T> & A, typename vector<T>::const_iterator b, typename vector<T>::const_iterator e,
					  typename vector<T>::size_type k, Compare __comp = std::less<T>()) {
	size_t n = e-b;
	if (n == 0) {
		return T();
	}
	if (n == 1) {
		return *b;
	}
	T pivot = *(b + (n-1)/2);
	if (n > 15) {
		vector<T> medians(n/5);
		for (size_t i = 0 ; i < medians.size(); i++) {
			medians[i] = quick_select(A,b+i*5,b+i*5+5,2,__comp);
		}
		pivot = quick_select(medians,medians.begin(),medians.end(),k/5,__comp);
	}

	vector<T> less, greater;
	less.reserve(n);
	greater.reserve(n);
	typename vector<T>::const_iterator it = b;
	for (; it != e; ++it) {
		if (*it == pivot) {
			continue;
		}
		if (__comp(*it,pivot)) {
			less.push_back(*it);
		}
		else {
			greater.push_back(*it);
		}
	}

	size_t q = less.size();
	if (q > k && !less.empty()) {
		return quick_select(less,less.begin(),less.end(),k,__comp);
	}
	if (q < k && !greater.empty()) {
		return quick_select(greater,greater.begin(),greater.end(),k-q-1,__comp);
	}
	return pivot;
}


template <typename T, class Compare >
static T median(const vector<T> & A,  typename vector<T>::const_iterator b, typename vector<T>::const_iterator e,
				Compare __comp = std::less<T>()) {
	return quick_select(A,b,e,(A.size()-1)/2,__comp);
}

template <typename T, class Compare >
static T median(const vector<T> & A, Compare __comp = std::less<T>()) {
	return median(A,A.begin(),A.end(),__comp);
}

template <typename T>
static T median(const vector<T> & A) {
	return median(A,std::less<T>());
}


