import i18n from "i18n-js";
import translations from "./translations.json";
import { locale } from "expo-localization";

i18n.locale = locale;
i18n.fallbacks = true;
i18n.fallback
i18n.translations = translations;

export const translate = i18n.t.bind(i18n)