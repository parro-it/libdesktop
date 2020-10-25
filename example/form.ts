import test from 'tape-async'
import {App,Window,Label,Textfield,Container,YogaFlexDirection} from '../index'
const app = App.create()


const win = new Window({},[   
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:6,top:6}},label: "A1"},[]),
        new Textfield({style: {margin:{left:6,top:6}},label: "A2"},[]),
    ]),
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:6,top:6}},label: "B1"},[]),
        new Textfield({style: {margin:{left:6,top:6}},label: "B2"},[]),
    ]),

    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:6,top:6}},label: "C1"},[]),
        new Textfield({style: {margin:{left:6,top:6}},label: "C2"},[]),
    ]),
]);

app.start();