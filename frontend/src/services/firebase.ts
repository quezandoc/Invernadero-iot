import { initializeApp } from "firebase/app";
import {
  getFirestore,
  collection,
  getDocs,
  onSnapshot,
} from "firebase/firestore";

// Tu configuración de Firebase
const firebaseConfig = {
  apiKey: "AIzaSyB-nP0BV0qwgK4tE_dMi2W2cihn8Z7rldQ",
  authDomain: "proyectoinvernadero-14daf.firebaseapp.com",
  databaseURL: "https://proyectoinvernadero-14daf-default-rtdb.firebaseio.com",
  projectId: "proyectoinvernadero-14daf",
  storageBucket: "proyectoinvernadero-14daf.appspot.com",
  messagingSenderId: "690797436592",
  appId: "1:690797436592:web:42a62d8fc5e37a2d062575",
  measurementId: "G-F8XJD7YVV6",
};

// Inicializa Firebase
const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

const lechugaRef = collection(db, "Lechuga");

// Obtener datos de la colección "Lechuga"
export const getLechugaData = async () => {
  const lechugaSnapshot = await getDocs(lechugaRef);
  const lechugaArray = lechugaSnapshot.docs.map((doc) => doc.data());
  const sortedData = lechugaArray.sort((a, b) => a.alive - b.alive);
  console.log(sortedData);
};

export const getLastLechugaData = async () => {
  const lechugaSnapshot = await getDocs(lechugaRef);
  const lechugaArray = lechugaSnapshot.docs.map((doc) => doc.data());
  const sortedData = lechugaArray.sort((a, b) => a.alive - b.alive);
  return sortedData[sortedData.length - 1];
};
