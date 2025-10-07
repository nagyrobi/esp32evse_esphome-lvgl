std::string formatNiceTime(const int& timeInSecs) {
  auto format_duration = [](int total_seconds) -> std::string {
    int days = total_seconds / 86400;
    int hours = (total_seconds % 86400) / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    std::string result;

    if (total_seconds < 60) {
      result = std::to_string(seconds) + "s";
    } else if (total_seconds < 3600) {
      if (minutes > 0) result += std::to_string(minutes) + "m";
      if (seconds > 0) {
        if (!result.empty()) result += " ";
        result += std::to_string(seconds) + "s";
      }
    } else if (total_seconds < 86400) {
      if (hours > 0) result += std::to_string(hours) + "h";
      if (minutes > 0) {
        if (!result.empty()) result += " ";
        result += std::to_string(minutes) + "m";
      }
    } else {
      if (days > 0) result += std::to_string(days) + "d";
      if (hours > 0) {
        if (!result.empty()) result += " ";
        result += std::to_string(hours) + "h";
      }
      if (minutes > 0) {
        if (!result.empty()) result += " ";
        result += std::to_string(minutes) + "m";
      }
    }

    return result;
  };

  return format_duration(timeInSecs);
}
