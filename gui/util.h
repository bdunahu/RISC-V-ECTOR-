/**
 * Given `total_levels', returns an integer between 4 and 12 which is a linear map of `level' onto `total_levels'.
 * This is used for generating cache sizes given a number of levels.
 * @param the total number of cache levels, zero-indexed.
 * @param a numberedcache level, zero-indexed.
 * @return an integer between 4-12, linearly scaled with level.
 */
int cache_size_mapper(int total_levels, int level);
